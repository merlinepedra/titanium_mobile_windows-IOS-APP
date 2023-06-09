/**
 * Copyright (c) 2015 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "TitaniumWindows/Network/HTTPClient.hpp"
#include "Titanium/detail/TiLogger.hpp"
#include "TitaniumWindows/Utility.hpp"

#include <collection.h>

using namespace concurrency;

namespace TitaniumWindows
{
	namespace Network
	{
		HTTPClient::HTTPClient(const JSContext& js_context) TITANIUM_NOEXCEPT
		    : Titanium::Network::HTTPClient(js_context)
		{
			TITANIUM_LOG_DEBUG("TitaniumWindows::Network::HTTPClient::ctor");
		}

		HTTPClient::~HTTPClient()
		{
			TITANIUM_LOG_DEBUG("TitaniumWindows::Network::HTTPClient::dtor");
			abort();

			filter__ = nullptr;
			httpClient__ = nullptr;
			dispatcherTimer__ = nullptr;
		}

		void HTTPClient::JSExportInitialize()
		{
			JSExport<HTTPClient>::SetClassVersion(1);
			JSExport<HTTPClient>::SetParent(JSExport<Titanium::Network::HTTPClient>::Class());
		}

		void HTTPClient::abort() TITANIUM_NOEXCEPT
		{
			cancellationTokenSource__.cancel();

			if (dispatcherTimer__ != nullptr && dispatcherTimer__->IsEnabled) {
				dispatcherTimer__->Stop();
			}
		}

		void HTTPClient::clearCookies(const std::string& location) TITANIUM_NOEXCEPT
		{
			auto filter = ref new Windows::Web::Http::Filters::HttpBaseProtocolFilter();

			Windows::Foundation::Uri ^ uri = ref new Windows::Foundation::Uri(TitaniumWindows::Utility::ConvertString(location));
			auto cookieCollection = filter->CookieManager->GetCookies(uri);

			Windows::Foundation::Collections::IIterator<Windows::Web::Http::HttpCookie^>^ iterator = cookieCollection->First();
			while (iterator->HasCurrent) {
				Windows::Web::Http::HttpCookie ^ cookie = iterator->Current;
				filter->CookieManager->DeleteCookie(cookie);
			}
		}

		std::string HTTPClient::getResponseHeader(const std::string& name) TITANIUM_NOEXCEPT
		{
			auto it = responseHeaders__.find(name.c_str());
			if (it != responseHeaders__.end()) {
				return it->second;
			} else {
				return "";
			}
		}

		void HTTPClient::open(const std::string& method, const std::string& location) TITANIUM_NOEXCEPT
		{
			method__ = Titanium::Network::RequestMethod::Get;
			if (method == "DELETE") {
				method__ = Titanium::Network::RequestMethod::Delete;
			} else if (method == "POST") {
				method__ = Titanium::Network::RequestMethod::Post;
			} else if (method == "PUT") {
				method__ = Titanium::Network::RequestMethod::Put;
			}

			location__ = location;
			filter__ = ref new Windows::Web::Http::Filters::HttpBaseProtocolFilter();
			httpClient__ = ref new Windows::Web::Http::HttpClient(filter__);
			cancellationTokenSource__ = concurrency::cancellation_token_source();
			filter__->AllowAutoRedirect = true;
			filter__->CacheControl->ReadBehavior = Windows::Web::Http::Filters::HttpCacheReadBehavior::MostRecent;

			addCookiesToRequest();
		}

		void HTTPClient::send() TITANIUM_NOEXCEPT
		{
			Windows::Web::Http::IHttpContent^ postData;
			send(postData);
		}

		void HTTPClient::send(const std::map<std::string, std::vector<std::uint8_t>>& postDataPairs, const bool& useMultipartForm) TITANIUM_NOEXCEPT
		{
			if (method__ == Titanium::Network::RequestMethod::Get) {
				TITANIUM_LOG_WARN("HTTPClient::send: Data found during a GET request. Method will be changed to POST.");
				method__ = Titanium::Network::RequestMethod::Post;
			}

			Windows::Web::Http::IHttpContent^ postData;
			if (useMultipartForm) {
				postData = ref new Windows::Web::Http::HttpMultipartFormDataContent();

				for (auto pair : postDataPairs) {
					Platform::String ^ name = TitaniumWindows::Utility::ConvertString(pair.first);
					static_cast<Windows::Web::Http::HttpMultipartFormDataContent^>(postData)->Add(ref new Windows::Web::Http::HttpBufferContent(charVecToBuffer(pair.second)), name);
				}
			} else {
				auto keyValues = ref new Platform::Collections::Map<Platform::String^, Platform::String^>();
				for (auto pair : postDataPairs) {
					Platform::String ^ name = TitaniumWindows::Utility::ConvertString(pair.first);

					std::stringstream ws_stream;
					for (auto c : pair.second) {
						ws_stream << c;
					}

					Platform::String ^ value = TitaniumWindows::Utility::ConvertString(ws_stream.str());
					keyValues->Insert(name, value);
				}

				postData = ref new Windows::Web::Http::HttpFormUrlEncodedContent(keyValues);
			}
			send(postData);
		}

		void HTTPClient::send(const std::string& postDataStr) TITANIUM_NOEXCEPT
		{
			Windows::Web::Http::IHttpContent^ postData = ref new Windows::Web::Http::HttpStringContent(TitaniumWindows::Utility::ConvertString(postDataStr));
			send(postData);
		}

		void HTTPClient::send(Windows::Web::Http::IHttpContent^ content)
		{
			auto uri = ref new Windows::Foundation::Uri(TitaniumWindows::Utility::ConvertString(location__));
			
			// Set up the request
			Windows::Web::Http::HttpRequestMessage^ request;
			if (method__ == Titanium::Network::RequestMethod::Post) {
				request = ref new Windows::Web::Http::HttpRequestMessage(Windows::Web::Http::HttpMethod::Post, uri);
				request->Content = content;
			} else if (method__ == Titanium::Network::RequestMethod::Put) {
				request = ref new Windows::Web::Http::HttpRequestMessage(Windows::Web::Http::HttpMethod::Put, uri);
				request->Content = content;
			} else if (method__ == Titanium::Network::RequestMethod::Delete) {
				request = ref new Windows::Web::Http::HttpRequestMessage(Windows::Web::Http::HttpMethod::Delete, uri);
			} else {
				request = ref new Windows::Web::Http::HttpRequestMessage(Windows::Web::Http::HttpMethod::Get, uri);
			}
			setRequestHeaders(request);

			auto operation = httpClient__->SendRequestAsync(request);

			// Startup a timer that will abort the request after the timeout period is reached.
			startDispatcherTimer();

			// clang-format off
			const auto token = cancellationTokenSource__.get_token();
			create_task(operation, token)
				.then([this, token](Windows::Web::Http::HttpResponseMessage^ response) {
				interruption_point();

				readyState__ = Titanium::Network::RequestState::Opened;
				onreadystatechange(readyState__);

				SerializeHeaders(response);

				return create_task(response->Content->ReadAsInputStreamAsync(), token);
			}, task_continuation_context::use_current())
				.then([this, token](Windows::Storage::Streams::IInputStream^ stream) {
				interruption_point();

				readyState__ = Titanium::Network::RequestState::Loading;
				onreadystatechange(readyState__);
				// FIXME Fire ondatastream/onsendstream callbacks throughout!

				return HTTPResultAsync(stream, token);
			}, task_continuation_context::use_current())
				.then([this](task<Windows::Storage::Streams::IBuffer^> previousTask) {
				try {
					// Check if any previous task threw an exception.
					previousTask.get();

					readyState__ = Titanium::Network::RequestState::Done;
					if (!disposed__ && httpClient__) {
						onreadystatechange(readyState__);

						// Fire onerror only if there's an onerror handler registered and status code is 400-599.
						// Otherwise fire onload (so 400-599 fall back to onload if no onerror handler)
						if (onerror__ && onerror__.IsObject() && static_cast<JSObject>(onerror__).IsFunction() && status__ >= 400 && status__ <= 599) {
       						onerror(status__, "HTTP Error", false);
       					} else {
       						onload(0, "Response has been loaded.", true);
       					}

						onsendstream(1.0);
						ondatastream(1.0);
					}
				}
				catch (const task_canceled&) {
					if (!disposed__ && httpClient__) {
						onerror(-1, "Session Cancelled", false);
					}
				}
				catch (Platform::Exception^ ex) {
					if (!disposed__ && httpClient__) {
						std::string error(TitaniumWindows::Utility::ConvertString(ex->Message));
						onerror(ex->HResult, error, false);
					}
				}
				catch (const std::exception& e) {
					if (!disposed__ && httpClient__) {
						std::string error(e.what());
						onerror(-1, error, false);
					}
				}
			});
			// clang-format on
		}

		void HTTPClient::setRequestHeader(const std::string& key, const std::string& value) TITANIUM_NOEXCEPT
		{
			auto it = requestHeaders__.find(key);
			if (it != requestHeaders__.end()) {
				requestHeaders__.erase(it);
			}

			requestHeaders__.insert(std::make_pair(key, value));
		}

		// properties
		std::string HTTPClient::get_allResponseHeaders() const TITANIUM_NOEXCEPT
		{
			std::stringstream s_stream;
			for (auto it = responseHeaders__.begin(); it != responseHeaders__.end(); ++it) {
				s_stream << it->first.c_str() << ":" << it->second.c_str() << ";";
			}

			return s_stream.str();
		}

		void HTTPClient::set_timeout(const std::chrono::milliseconds& timeout) TITANIUM_NOEXCEPT
		{
			std::chrono::duration<std::chrono::nanoseconds::rep, std::ratio_multiply<std::ratio<100>, std::nano>> timer_interval_ticks = timeout;
			timeoutSpan__.Duration = timer_interval_ticks.count();
		}

		// Native
		void HTTPClient::setRequestHeaders(Windows::Web::Http::HttpRequestMessage^ request) TITANIUM_NOEXCEPT
		{
			// Set type safe headers. Content-Type is the most popular may need to set others.
			auto it = requestHeaders__.find("Content-Type");
			if (it != requestHeaders__.end()) {
				request->Content->Headers->ContentType =
					ref new Windows::Web::Http::Headers::HttpMediaTypeHeaderValue(TitaniumWindows::Utility::ConvertString(it->second));
				requestHeaders__.erase(it);
			}

			for (it = requestHeaders__.begin(); it != requestHeaders__.end(); ++it) {
				auto key = TitaniumWindows::Utility::ConvertString(it->first);
				// ignore cookies they are added during open to the request filter.
				if (!key->Equals("Cookie")) {
					auto value = it->second;
					if (!value.empty()) {
						request->Headers->Append(key, TitaniumWindows::Utility::ConvertString(value));
					} else {
						if (request->Headers->HasKey(key)) {
							request->Headers->Remove(key);
						}
					}
				}
			}
		}

		task<Windows::Storage::Streams::IBuffer^> HTTPClient::HTTPResultAsync(Windows::Storage::Streams::IInputStream^ stream, concurrency::cancellation_token token)
		{
			Windows::Storage::Streams::IBuffer^ responseBuffer = ref new Windows::Storage::Streams::Buffer(1000); // Default size from MS samples
			// clang-format off
			return create_task(stream->ReadAsync(responseBuffer, responseBuffer->Capacity, Windows::Storage::Streams::InputStreamOptions::Partial), token)
				.then([=](task<Windows::Storage::Streams::IBuffer^> readTask) {

				if (token.is_canceled()) {
					cancel_current_task();
				}

				// Stop the timeout timer
				if (dispatcherTimer__ != nullptr && httpClient__ != nullptr) {
					dispatcherTimer__->Stop();
				}

				if (contentLength__ != -1 && contentLength__ != 0) {
					ondatastream(responseBuffer->Length / contentLength__);
				} else {
					ondatastream(-1.0); // chunked encoding was used
				}

				if (responseBuffer->Length) {
					auto reader = ::Windows::Storage::Streams::DataReader::FromBuffer(responseBuffer);
					responseData__.resize(responseDataLen__ + responseBuffer->Length);
					reader->ReadBytes(
						::Platform::ArrayReference<std::uint8_t>(
						&responseData__[responseDataLen__], responseBuffer->Length));
					responseDataLen__ += responseBuffer->Length;
				}
				
				// FIXME How do we pass the token on in case of readTask?
				return responseBuffer->Length ? HTTPResultAsync(stream, token) : readTask;
			}, task_continuation_context::use_current());
			// clang-format on
		}

		void HTTPClient::startDispatcherTimer()
		{
			if (dispatcherTimer__ == nullptr && timeoutSpan__.Duration > 0) {
				dispatcherTimer__ = ref new Windows::UI::Xaml::DispatcherTimer();
				dispatcherTimer__->Interval = timeoutSpan__;
				auto timeoutRegistrationToken__ = dispatcherTimer__->Tick += ref new Windows::Foundation::EventHandler<Platform::Object^>([this](Platform::Object^ sender, Platform::Object^ e) {
					cancellationTokenSource__.cancel();
					dispatcherTimer__->Stop();
					// re-create the CancellationTokenSource.
					cancellationTokenSource__ = cancellation_token_source();
				});
				dispatcherTimer__->Start();
			}
		}

		Windows::Storage::Streams::Buffer^ HTTPClient::charVecToBuffer(std::vector<std::uint8_t> char_vector)
		{
			int size = char_vector.size();

			Windows::Storage::Streams::Buffer^ buffer = ref new Windows::Storage::Streams::Buffer(size);
			buffer->Length = size;

			Microsoft::WRL::ComPtr<Windows::Storage::Streams::IBufferByteAccess> bufferByteAccess;
			HRESULT hr = reinterpret_cast<IUnknown*>(buffer)->QueryInterface(IID_PPV_ARGS(&bufferByteAccess));
			if (FAILED(hr)) {
				throw ref new Platform::Exception(hr);
			}

			std::uint8_t* data = (std::uint8_t*)&char_vector[0];
			hr = bufferByteAccess->Buffer(&data);
			if (FAILED(hr)) {
				throw ref new Platform::Exception(hr);
			}

			return buffer;
		}

		void HTTPClient::SerializeHeaders(Windows::Web::Http::HttpResponseMessage^ response)
		{
			status__ = static_cast<std::uint32_t>(response->StatusCode);

			SerializeHeaderCollection(response->Headers);
			SerializeHeaderCollection(response->Content->Headers);

			std::map<std::string, std::string>::iterator it;
			it = responseHeaders__.find("Content-Length");
			if (it != responseHeaders__.end()) {
				contentLength__ = atol(it->second.c_str());
			} else {
				contentLength__ = -1;  // chunked encoding
			}

			readyState__ = Titanium::Network::RequestState::Headers_Received;
			onreadystatechange(Titanium::Network::RequestState::Headers_Received);
		}

		void HTTPClient::SerializeHeaderCollection(Windows::Foundation::Collections::IIterable<Windows::Foundation::Collections::IKeyValuePair<Platform::String^, Platform::String^>^>^ headers)
		{
			for each(Windows::Foundation::Collections::IKeyValuePair<Platform::String^, Platform::String^>^ pair in headers)
			{
				responseHeaders__.insert(std::make_pair(TitaniumWindows::Utility::ConvertString(pair->Key), TitaniumWindows::Utility::ConvertString(pair->Value)));
			}
		}

		void HTTPClient::addCookiesToRequest()
		{
			auto location = TitaniumWindows::Utility::ConvertString(location__);

			for (auto it = requestHeaders__.begin(); it != requestHeaders__.end(); ++it) {
				
				auto key   = TitaniumWindows::Utility::ConvertString(it->first);
				auto value = TitaniumWindows::Utility::ConvertString(it->second);

				if (key->Equals("Cookie")) {
					//////////////////////////////////////////////////////////////////
					try {
						auto cookie = ref new Windows::Web::Http::HttpCookie(key, location, "/");

						char* context = nullptr;
						char* pvalue = strtok_s((char*)it->second.c_str(), "=", &context);
						if (pvalue != nullptr) {
							std::string str = std::string(pvalue);
							cookie->Value = TitaniumWindows::Utility::ConvertString(str);
							cookie->Expires = nullptr;
							cookie->Secure = false;
							cookie->HttpOnly = true;
							filter__->CookieManager->SetCookie(cookie, false);
						}
					} catch (Platform::InvalidArgumentException^ ex) {
						std::string error(TitaniumWindows::Utility::ConvertString(ex->Message));
						onerror(ex->HResult, error, false);
					} catch (Platform::Exception^ ex) {
						std::string error(TitaniumWindows::Utility::ConvertString(ex->Message));
						onerror(ex->HResult, error, false);
					}
				}
			}
		}
	}
}
