/*
 * Appcelerator Titanium Mobile
 * Copyright (c) 2011-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */
var should = require('./should');

describe("Titanium.UI.TextField", function () {
    it("value", function (finish) {
        var textfield = Ti.UI.createTextField({
            value: "this is some text"
        });
        should(textfield.value).be.a.String;
        should(textfield.getValue).be.a.Function;
        should(textfield.value).eql('this is some text');
        should(textfield.getValue()).eql('this is some text');
        textfield.value = 'other text';
        should(textfield.value).eql('other text');
        should(textfield.getValue()).eql('other text');
        finish();
    });

    it("textAlign", function (finish) {
        var textfield = Ti.UI.createTextField({
            value: "this is some text",
            textAlign: Titanium.UI.TEXT_ALIGNMENT_CENTER
        });
        should(textfield.textAlign).be.a.Number; // String on Android
        should(textfield.getTextAlign).be.a.Function;
        should(textfield.textAlign).eql(Titanium.UI.TEXT_ALIGNMENT_CENTER);
        should(textfield.getTextAlign()).eql(Titanium.UI.TEXT_ALIGNMENT_CENTER);
        textfield.textAlign = Titanium.UI.TEXT_ALIGNMENT_RIGHT;
        should(textfield.textAlign).eql(Titanium.UI.TEXT_ALIGNMENT_RIGHT);
        should(textfield.getTextAlign()).eql(Titanium.UI.TEXT_ALIGNMENT_RIGHT);
        finish();
    });
    it("verticalAlign", function (finish) {
        var textfield = Ti.UI.createTextField({
            value: "this is some text",
            verticalAlign: Titanium.UI.TEXT_VERTICAL_ALIGNMENT_BOTTOM
        });
        should(textfield.verticalAlign).be.a.Number; // String on Android
        should(textfield.getVerticalAlign).be.a.Function;
        should(textfield.verticalAlign).eql(Titanium.UI.TEXT_VERTICAL_ALIGNMENT_BOTTOM);
        should(textfield.getVerticalAlign()).eql(Titanium.UI.TEXT_VERTICAL_ALIGNMENT_BOTTOM);
        textfield.verticalAlign = Titanium.UI.TEXT_VERTICAL_ALIGNMENT_TOP;
        should(textfield.verticalAlign).eql(Titanium.UI.TEXT_VERTICAL_ALIGNMENT_TOP);
        should(textfield.getVerticalAlign()).eql(Titanium.UI.TEXT_VERTICAL_ALIGNMENT_TOP);
        finish();
    });

    // TODO Add tests for:
    // autocapitalize
    // autocorrect
    // borderStyle
    // clearonEdit
    // color
    // editable
    // enableReturnKey
    // font
    // hintText
    // keyboardType
    // maxLength
    // passwordMask
    // returnKeyType
    // selection
    // suppressReturn

    it.skip("width", function (finish) {
        this.timeout(1000);
        var textfield = Ti.UI.createTextField({
            value: 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec nec ullamcorper massa, eget tempor sapien. Phasellus nisi metus, tempus a magna nec, ultricies rutrum lacus. Aliquam sit amet augue suscipit, dignissim tellus eu, consectetur elit. Praesent ligula velit, blandit vel urna sit amet, suscipit euismod nunc.',
            width: Ti.UI.SIZE
        });
        var win = Ti.UI.createWindow({
            backgroundColor: '#ddd'
        });
        win.add(textfield);
        win.addEventListener('postlayout', function () {
            should(win.width).be.greaterThan(100);
            should(textfield.width).not.be.greaterThan(win.width);
            finish();
        });
        win.open();
    });

    it.skip("height", function (finish) {
        this.timeout(1000);
        var textfield = Ti.UI.createTextField({
            value: 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec nec ullamcorper massa, eget tempor sapien. Phasellus nisi metus, tempus a magna nec, ultricies rutrum lacus. Aliquam sit amet augue suscipit, dignissim tellus eu, consectetur elit. Praesent ligula velit, blandit vel urna sit amet, suscipit euismod nunc.',
            width: Ti.UI.SIZE,
            height: Ti.UI.SIZE,
            color: 'black'
        });
        var bgView = Ti.UI.createView({
            width: 200, height: 100,
            backgroundColor: 'red'
        });
        var win = Ti.UI.createWindow({
            backgroundColor: '#eee'
        });
        bgView.add(textfield)
        win.add(bgView);

        win.addEventListener('postlayout', function () {
            should(bgView.height).be.eql(100);
            should(textfield.height).not.be.greaterThan(100);
            finish();
        });
        win.open();
    });

});
