// WysiBB - WYSIWYG BBCode editor - v1.0.2 - 2012-09-03
// http://www.wysibb.com
// Copyright (c) 2012 Vadim Dobroskok; Licensed MIT, GPL
UWBB_VER = '1.1';
if( typeof WBBLANG == 'undefined' ) {
	WBBLANG = {};
}
var CURLANG = {};
( function($) {
	'use strict';
	$.wysibb = function(txtArea, settings) {
		$(txtArea).data('wbb', this);
		if(settings && settings.lang && typeof WBBLANG[settings.lang] != 'undefined') {
			CURLANG = WBBLANG[settings.lang];
		}
		this.txtArea = txtArea;
		this.$txtArea = $(txtArea);
		var id = this.$txtArea.attr('id') || this.setUID(this.txtArea);
		this.options = {
			bbmode: false,
			onlyBBmode: false,
			themeName: 'default',
			bodyClass: '',
			lang: 'ru',
			tabInsert: true,
			buttons: '',
			allButtons: {
				bold : {
					title: CURLANG.bold,
					buttonHTML: '<span class="ve-tlb-bold"></span>',
					excmd: 'bold',
					hotkey: 'ctrl+b',
					transform : {
						'<b>{SELTEXT}</b>':"[b]{SELTEXT}[/b]",
						'<strong>{SELTEXT}</strong>':"[b]{SELTEXT}[/b]"
					}
				},
				italic : {
					title: CURLANG.italic,
					buttonHTML: '<span class="ve-tlb-italic"></span>',
					excmd: 'italic',
					hotkey: 'ctrl+i',
					transform : {
						'<i>{SELTEXT}</i>':"[i]{SELTEXT}[/i]",
						'<em>{SELTEXT}</em>':"[i]{SELTEXT}[/i]"
					}
				},
				underline : {
					title: CURLANG.underline,
					buttonHTML: '<span class="ve-tlb-underline"></span>',
					excmd: 'underline',
					hotkey: 'ctrl+u',
					transform : {
						'<u>{SELTEXT}</u>':"[u]{SELTEXT}[/u]"
					}
				},
				strike : {
					title: CURLANG.strike,
					buttonHTML: '<span class="ve-tlb-strike"></span>',
					excmd: 'strikeThrough',
					transform : {
						'<strike>{SELTEXT}</strike>':"[s]{SELTEXT}[/s]",
						'<s>{SELTEXT}</s>':"[s]{SELTEXT}[/s]"
					}
				},
				sup : {
					title: CURLANG.sup,
					buttonHTML: '<span class="ve-tlb-sup"></span>',
					excmd: 'superscript',
					transform : {
						'<sup>{SELTEXT}</sup>':"[sup]{SELTEXT}[/sup]"
					}
				},
				sub : {
					title: CURLANG.sub,
					buttonHTML: '<span class="ve-tlb-sub"></span>',
					excmd: 'subscript',
					transform : {
						'<sub>{SELTEXT}</sub>':"[sub]{SELTEXT}[/sub]"
					}
				},
				fontcolor: {
					type: "colorpicker",
					title: CURLANG.fontcolor,
					excmd: "foreColor",
					valueBBname: "color",
					subInsert: true,
					colors: "#000000,#444444,#666666,#999999,#b6b6b6,#cccccc,#d8d8d8,#efefef,#f4f4f4,#ffffff,-, \
							 #ff0000,#980000,#ff7700,#ffff00,#00ff00,#00ffff,#1e84cc,#0000ff,#9900ff,#ff00ff,-, \
							 #f4cccc,#dbb0a7,#fce5cd,#fff2cc,#d9ead3,#d0e0e3,#c9daf8,#cfe2f3,#d9d2e9,#ead1dc, \
							 #ea9999,#dd7e6b,#f9cb9c,#ffe599,#b6d7a8,#a2c4c9,#a4c2f4,#9fc5e8,#b4a7d6,#d5a6bd, \
							 #e06666,#cc4125,#f6b26b,#ffd966,#93c47d,#76a5af,#6d9eeb,#6fa8dc,#8e7cc3,#c27ba0, \
							 #cc0000,#a61c00,#e69138,#f1c232,#6aa84f,#45818e,#3c78d8,#3d85c6,#674ea7,#a64d79, \
							 #900000,#85200C,#B45F06,#BF9000,#38761D,#134F5C,#1155Cc,#0B5394,#351C75,#741B47, \
							 #660000,#5B0F00,#783F04,#7F6000,#274E13,#0C343D,#1C4587,#073763,#20124D,#4C1130",
					transform: {
						'<font color="{COLOR}">{SELTEXT}</font>':'[color={COLOR}]{SELTEXT}[/color]'
					}
				},
				smilebox: {
					type: 'smilebox',
					title: CURLANG.smilebox,
					buttonHTML: '<span class="ve-tlb-smilebox"></span>'
				}
			},
			systr: {
				'<br/>':"\n",
				'<span class="wbbtab"></span>': '   '
			},
			customRules: {},
			smileList: false,
			attrWrap: [
				'src',
				'color',
				'href'
			]
		};
		if( ! this.options.themePrefix ) {
			$('link').each( $.proxy( function(idx, el) {
				var sriptMatch = $(el).get(0).href.match(/(.*\/)(.*)\/wbbtheme\.css.*$/);
				if(sriptMatch !== null) {
					this.options.themeName = sriptMatch[2];
					this.options.themePrefix = sriptMatch[1];
				}
			}, this ) );
		}
		if( typeof WBBPRESET != 'undefined' ) {
			if( WBBPRESET.allButtons ) {
				$.each( WBBPRESET.allButtons, $.proxy( function(k,v) {
					if( v.transform && this.options.allButtons[k] ) {
						delete this.options.allButtons[k].transform;
					}
				}, this ) );
			}
			$.extend(true, this.options, WBBPRESET);
		} 
		if( settings && settings.allButtons ) {
			$.each( settings.allButtons, $.proxy( function(k,v) {
				if(v.transform && this.options.allButtons[k]) {
					delete this.options.allButtons[k].transform;
				}
			}, this ) );
		}
		$.extend(true, this.options, settings);
		this.init();
	};
	$.wysibb.prototype = {
		lastid: 1,
		init: function() {
			if( this.options.preInit && $.isFunction( this.options.preInit ) ) {
				this.options.preInit.call(this);
			}
			if( typeof console != 'undefined' && typeof console.log  != 'undefined' ) {
				//~ console.log('WysiBB v. 1.0.2 (2012-09-03); (c) 2012 Vadim Dobroskok (MIT, GPL); http://www.wysibb.com\nAdopted for uCoz: uwbb [' + UWBB_VER + ']');
				console.log('uwbb [' + UWBB_VER + ']');
			}
			this.isMobile = /android.+mobile|avantgo|bada\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|iris|kindle|lge |maemo|meego.+mobile|midp|mmp|netfront|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test( navigator.userAgent || navigator.vendor || window.opera );
			if(this.isMobile) {
				this.onlyBBmode = true;
				this.bbmode = true;
			}
			this.controllers = [];
			if( typeof this.options.bitmask != 'number' ) {
				this.options.bitmask = 0;
			}
			this.options.buttons = this.options.buttons.toLowerCase();
			this.options.buttons = this.options.buttons.split(',');
			this.options.allButtons["_systr"] = {};
			this.options.allButtons["_systr"]["transform"] = this.options.systr;
			this.smileFind();
			this.initTransforms();
			this.build();
			this.initModal();
			if( this.options.hotkeys === true && ! this.isMobile ) {
				this.initHotkeys();
			}
			this.options.smileList.sort( function(a, b) {
				return ( b.bbcode.length - a.bbcode.length );
			} );
			this.$txtArea.parents('form').bind( 'submit', $.proxy( function() {
				this.autosave();
				this.sync();
				return $.browser.msie && document.doctype ? false : true;
			}, this ) );
			this.uCozIntegration();
			this.$txtArea.parents('form').find('input[type="reset"]').click( $.proxy( function() {
				this.$body.empty();
				this.sync();
				if( this.options.bbmode ) {
					this.$txtArea.get(0).focus();
				} else {
					this.$body.get(0).focus();
				}
			}, this ) );
			if( this.options.postInit && $.isFunction( this.options.postInit ) ) {
				this.options.postInit.call(this);
			}
		},
		uImages: {},
		uImagesToHTML: function(bbdata) {
			for( var number in this.uImages ) {
				if( ! this.uImages[number].image ) {
					continue;
				}
				bbdata = bbdata.replace( new RegExp('\\$IMAGE' + number + '\\$', 'g'), this.uImages[number].image.outerHTML );
			}
			return bbdata;
		},
		uImagesInit: function() {
			if( ! window.FileReader ) {
				return;
			}
			if( ! this.$txtArea.parents('form').find('#iplus') ) {
				return;
			}
			var uwbb = this;
			this.$txtArea.parents('form').find('.ucoz-forum-post-image-info').each( function() {
				if( /\.(png|jpg|gif|jpeg|jpe)$/i.test(this.href) ) {
					var number;
					if( number = /ucoz-forum-post-image-info-(\d+)/.exec(this.className) ) {
						number = parseInt( number[1] );
						uwbb.uImages[number] = {};
						uwbb.uImages[number].image = $('<img/>').addClass('ucoz-forum-post-image ucoz-forum-post-image-' + number).css( {
							'max-width': '500px',
							'max-height': '400px'
						} ).attr('src', this.href).get(0);
						$(this).addClass('ulightbox');
					}
				}
			} );
			if( typeof window.getimage == 'function' && typeof window._getimage != 'function' ) {
				window._getimage = window.getimage;
				window.getimage = function(number) {
					number = parseInt(number);
					if( ! window.FileReader ) {
						return window._getimage.call(this, number);
					}
					var input = $('#fln' + number).get(0);
					if( ! input ) {
						return window._getimage.call(this, number);
					}
					var file = input.files[0];
					if( ! ( file && /^image\/*/.test(file.type) ) ) {
						if( uwbb.uImages[number] ) {
							uwbb.uImages[number].hide();
							uwbb.$body.find('.ucoz-forum-post-image-' + number).remove();
						}
						return window._getimage.call(this, number);
					}
					if( uwbb.uImages[number] && uwbb.uImages[number].reader ) {
						uwbb.uImages[number].reader.readAsDataURL(file);
						uwbb.uImages[number].show();
						return;
					}
					uwbb.uImages[number] = {};
					uwbb.uImages[number].number = number;
					uwbb.uImages[number].input = input;
					uwbb.uImages[number].interface = $('<span/>').addClass('ucoz-forum-post-image-interface').insertBefore(input);
					uwbb.uImages[number].preview = $('<img/>').addClass('ucoz-forum-post-image-preview').css( {
						'max-height': '24px',
						'max-width': '24px',
						'vertical-align': 'middle',
						'cursor': 'pointer'
					} ).click( function() {
						$.fancybox( {
							'content': $('<img/>').attr('src', this.src)
						} );
					} ).appendTo(uwbb.uImages[number].interface).get(0);
					var filterNumber = function(event) {
						return ( event.which <= 57 && event.which >= 48 ) || event.which == 8 || event.which == 9 || event.which == 46 || event.which == 37 || event.which == 39 || ( event.ctrlKey && event.which == 65 ) ? true : false;
					};
					var setHeight = function() {
						$(uwbb.uImages[number].image).css('max-height', this.value + 'px');
						uwbb.$body.find('.ucoz-forum-post-image-' + number).css('max-height', this.value + 'px');
					};
					var setWidth = function() {
						$(uwbb.uImages[number].image).css('max-width', this.value + 'px');
						uwbb.$body.find('.ucoz-forum-post-image-' + number).css('max-width', this.value + 'px');
					};
					uwbb.uImages[number].image = $('<img/>').addClass('ucoz-forum-post-image ucoz-forum-post-image-' + number).css( {
						'max-width': '500px',
						'max-height': '400px'
					} ).get(0);
					uwbb.uImages[number].insert = $('<a/>').addClass('ucoz-forum-post-image-insert').css( {
						'margin': '0 0 0 5px'
					} ).attr( {
						'href': 'javascript://'
					} ).text(UWBBLANG.uImageInsert).click( function() {
						uwbb.insertAtCursor( uwbb.options.bbmode ? '$IMAGE' + number + '$' : uwbb.uImages[number].image.outerHTML );
					} ).appendTo( uwbb.uImages[number].interface );
					uwbb.uImages[number].remove = $('<a/>').addClass('ucoz-forum-post-image-remove').css( {
						'margin': '0 5px'
					} ).attr( {
						'href': 'javascript://'
					} ).text(UWBBLANG.uImageRemove).click( function() {
						$('#del_img' + number).attr('checked', true);
						uwbb.uImages[number].hide();
						uwbb.uImages[number].input.value = null;
						uwbb.$body.find('.ucoz-forum-post-image-' + number).remove();
					} ).appendTo( uwbb.uImages[number].interface );
					var imageMaxWidth = parseInt(uwbb.options.imageWidth);
						imageMaxWidth = imageMaxWidth.toString().toLowerCase() == 'nan' ? 0 : imageMaxWidth;
					var imageMaxHeight = parseInt(uwbb.options.imageHeight);
						imageMaxHeight = imageMaxHeight.toString().toLowerCase() == 'nan' ? 0 : imageMaxHeight;
					uwbb.uImages[number].width = $('<input/>').css( {
						'font-size': '7pt',
						'color': 'gray',
						'width': '40px'
					} ).attr( {
						'type': 'text',
						'name': 'iws' + number,
						'value': imageMaxWidth,
						'maxlength': 4,
						'title': UWBBLANG.uImageWidth
					} ).keydown(filterNumber).keyup(setWidth).appendTo( uwbb.uImages[number].interface ).get(0);
					$('<span/>').html('&nbsp;x&nbsp;').appendTo( uwbb.uImages[number].interface );
					uwbb.uImages[number].height = $('<input/>').css( {
						'font-size': '7pt',
						'color': 'gray',
						'width': '40px'
					} ).attr( {
						'type': 'text',
						'name': 'ihs' + number,
						'value': imageMaxHeight,
						'maxlength': 4,
						'title': UWBBLANG.uImageHeight
					} ).keydown(filterNumber).keyup(setHeight).appendTo( uwbb.uImages[number].interface ).get(0);
					setHeight.call(uwbb.uImages[number].height);
					setWidth.call(uwbb.uImages[number].width);
					uwbb.uImages[number].hide = function() {
						this.interface.hide();
					};
					uwbb.uImages[number].reader = new FileReader();
					uwbb.uImages[number].reader.onload = function() {
						uwbb.uImages[number].preview.src = this.result;
						uwbb.uImages[number].image.src = this.result;
						uwbb.$body.find('.ucoz-forum-post-image-' + number).attr('src', this.result);
					}
					uwbb.uImages[number].show = function() {
						this.interface.show();
					};
					uwbb.uImages[number].reader.readAsDataURL(file);
					uwbb.uImages[number].show();
				}
			}
		},
		uCozFunctions: {
			'prepost': false,
			'addcom': false,
			'gbsafe': false,
			'sendIpBnF': false,
			'safe': false,
			'sendFrm235': true
		},
		uCozIntegration: function() {
			if( typeof window.uCoz.uwbb.reAssigned != 'object' || window.uCoz.uwbb.reAssigned == null ) {
				window.uCoz.uwbb.reAssigned = {};
			}
			if( typeof window.uCoz.uwbb.syncAll != 'function' ) {
				window.uCoz.uwbb.syncAll = function() {
					for( var instance in this.instances ) {
						try {
							this.instances[instance].sync();
						} catch(e) {}
					}
				}
			}
			for( var functionName in this.uCozFunctions ) {
				if( typeof window[functionName] == 'function' && ( this.uCozFunctions[functionName] || typeof window.uCoz.uwbb.reAssigned[functionName] == 'undefined' ) ) {
					window.uCoz.uwbb.reAssigned[functionName] = window[functionName];
					eval( 'window.' + functionName + ' = function() { window.uCoz.uwbb.syncAll(); window.uCoz.uwbb.reAssigned.' + functionName + '.call(this, arguments[0], arguments[1]); };' );
				}
			}
		},
		initTransforms: function() {
			var o = this.options;
			if( ! o.rules ) {
				o.rules = {};
			}
			var btnlist = o.buttons.slice();
			for( var bidx = 0; bidx < btnlist.length; bidx++ ) {
				var ob = o.allButtons[ btnlist[bidx] ];
				if( ! ob ) {
					continue;
				}
				if( ob.type == 'select' && typeof ob.options == 'string' ) {
					var olist = ob.options.split(',');
					$.each( olist, function(i, op) {
						if($.inArray(op, btnlist) == -1 ) {
							btnlist.push(op);
						}
					} );
				}
				if( ob.transform && ob.skipRules !== true ) {
					for( var bhtml in ob.transform ) {
						var bbcode = ob.transform[bhtml];
						$.each( o.attrWrap, function(i, a) {
							bhtml = bhtml.replace( a + '="', '_' + a + '="' );
						} );
						var $bel = $( document.createElement('DIV') ).append( $( this.elFromString(bhtml, document) ) );
						var rootSelector = this.filterByNode( $bel.children() );
						if( ! ob.excmd ) {
							if( ! ob.rootSelector ) {
								ob.rootSelector = [];
							}
							ob.rootSelector.push(rootSelector);
						}
						if( ! ob.bbSelector ) {
							ob.bbSelector = [];
						}
						if( $.inArray(bbcode, ob.bbSelector) == -1 ) {
							ob.bbSelector.push(bbcode);
						}
						if( typeof o.rules[rootSelector] == 'undefined' ) {
							o.rules[rootSelector] = [];
						}
						var crules = {};
						if( bhtml.match(/\{\S+?\}/) ) {
							$bel.find('*').each( $.proxy( function(idx, el) {
								var attributes = this.getAttributeList(el);
								$.each( attributes, $.proxy( function(i, item) {
									var attr = $(el).attr(item);
									if( item.substr(0, 1) == '_' ) {
										item = item.substr(1);
									}
									var r = attr.match(/\{\S+?\}/g);
									if(r) {
										for( var a = 0; a < r.length; a++ ) {
											var rname = r[a].substr( 1, r[a].length - 2 );
											var p = this.relFilterByNode(el,rootSelector);
											var regRepl = attr != r[a] ? this.getRegexpReplace( attr, r[a] ) : false;
											crules[ rname.toLowerCase() ] = { sel: p ? $.trim(p) : false, attr: item, rgx: regRepl }
										}
									}
								}, this ) );
								var sl = [];
								if( ! $(el).is('iframe') ) {
									$(el).contents().filter( function() {
										return this.nodeType === 3;
									} ).each( $.proxy( function(i, rel) {
										var txt = rel.textContent || rel.data;
										if( typeof(txt) == 'undefined' ) {
											return true;
										}
										var r = txt.match(/\{\S+?\}/g)
										if(r) {
											for( var a = 0; a < r.length; a++ ) {
												var rname = r[a].substr( 1, r[a].length - 2 );
												var p = this.relFilterByNode(el, rootSelector);
												var regRepl = txt != r[a] ? this.getRegexpReplace( txt, r[a] ) : false;
												var sel = p ? $.trim(p) : false;
												if( $.inArray(sel, sl) > -1 || $(rel).parent().contents().size() > 1 ) {
													var nel = $('<span>').html( '{' + rname + '}' );
													this.setUID(nel, 'wbb');
													var start = ( txt.indexOf(rname) + rname.length ) + 1;
													var after_txt = txt.substr( start, txt.length - start );
													rel.data = txt.substr( 0, txt.indexOf(rname) - 1 );
													$(rel).after( this.elFromString(after_txt, document) ).after(nel);
													sel= ( sel ? sel + ' ' : '' ) + this.filterByNode(nel);
													regRepl = false;
												}
												crules[ rname.toLowerCase() ] = { sel: sel, attr:false, rgx:regRepl };
												sl[ sl.length ] = sel;
											}
										}
									}, this ) );
								}
								sl = null;
								var nbhtml = $bel.html();
								if( bhtml != nbhtml ) {
									delete ob.transform[bhtml];
									ob.transform[nbhtml] = bbcode;
									bhtml = nbhtml;
								}
							}, this ) );
						}
						o.rules[rootSelector].push( [bbcode, crules] );
					}
					var htmll = $.map( ob.transform, function(bb, html) { return html; } ).sort( function(a, b) {
						return ( ( b[0] || '' ).length - ( a[0] || '' ).length );
					} );
					ob.bbcode = ob.transform[ htmll[0] ];
					ob.html = htmll[0];
				}
			};
			this.options.btnlist = btnlist;
			$.extend( o.rules, this.options.customRules );
			$.each( o.systr, $.proxy(function(html, bb) {
				if( ! html.match(/\{\S+?\}/) ) {
					var rs = this.filterByNode( this.elFromString(html, document) );
					o.rules[rs] = [];
					o.rules[rs].push( [ bb, {} ] );
				}
			}, this ) );
			o.srules = {};
			$.each( o.smileList, $.proxy( function(i, sm) {
				var $sm = $( this.strf(sm.img, o) );
				var f = this.filterByNode($sm);
				o.srules[f] = [sm.bbcode, sm.img];
			}, this ) );
			for( var rootsel in o.rules ) {
				this.options.rules[rootsel].sort( function(a, b) {
					return ( b[0].length - a[0].length );
				} );
			}
		},
		build: function() {
			this.$editor = $('<div/>').addClass('wysibb');
			this.$editor.insertAfter(this.txtArea).append(this.txtArea);
			this.$txtArea.addClass('wysibb-texarea');
			this.buildToolbar();
			this.$txtArea.wrap('<div class="wysibb-text"></div>');
			this.uImagesInit();
			if( this.options.onlyBBmode === false ) {
				var height = this.$txtArea.outerHeight();
				this.$iFrame = $( this.strf( '<iframe src="about:blank" class="wysibb-text-iframe" frameborder="0" style="height:{height}px;"></iframe>', { height: height } ) );
				this.$txtArea.hide();
				this.$iFrame.bind('load', $.proxy( function() {
					this.framewindow = this.$iFrame.get(0).contentWindow;
					this.doc = this.framewindow.document;
					this.body = this.doc.body;
					this.$body = $(this.doc.body);
					this.$body.css('height', 'auto');
					var ihead = this.doc.getElementsByTagName('head')[0];
					this.$body.addClass('wysibb-body' + ( $.browser.msie ? ' wysibb-body-ie' : '' ) ).addClass( this.options.bodyClass );
					$("link[data-type='uwbb']").each( function(idx, el) {
						$(ihead).append( $(el).clone()[0].outerHTML );
					} );
					if( typeof this.options.cssFile == 'string' && this.options.cssFile.length ) {
						$( '<link/>', {
							'type': 'text/css',
							'rel': 'stylesheet',
							'href': this.options.cssFile
						} ).appendTo(ihead);
					} else {
						$(ihead).append( $(document).find('link[href*=".css"]').clone() );
					}
					if( 'contentEditable' in this.body ) {
						this.body.contentEditable = true;
						try {
							this.doc.execCommand('StyleWithCSS', false, false);
							if( $.browser.opera ) {
								this.doc.execCommand('opera-defaultblock', '', 'div');
							}
						} catch(e) {}
					} else {
						this.options.onlyBBmode = true;
						this.options.bbmode = true;
					}
					if( this.txtArea.value.length > 0 ) {
						this.$body.html( this.getHTML(this.txtArea.value, true) );
					}
					$(this.doc).bind( 'keydown', $.proxy( function(e) {
						if( e.which == 8 && ! this.$body.text().length && this.$body.find('img').length < 2 ) {
							this.$toolbar.find('.wbb-bold.on, .wbb-italic.on, .wbb-underline.on, .wbb-strike.on, .wbb-sub.on, .wbb-sup.on').click();
						}
						this.$iFrame.contents().find('legend br').remove();
					}, this ) );
					if( ! $.browser.opera ) $(this.doc).bind('keydown', $.proxy( function(e) {
						if( ( e.which == 86 && ( e.ctrlKey == true || e.metaKey == true ) ) || ( e.which == 45 && ( e.shiftKey == true || e.metaKey == true ) ) ) {
							var rng = this.getRange();
							var intoCode = this.queryState('code') ? true : false;
							this.$body.removeAttr('contentEditable');
							var $tmpel = $('<div/>');
							this.lastRange = this.getRange();
							$tmpel.attr('contenteditable', 'true').attr('class', 'paste').appendTo(this.body).focus();
							setTimeout( $.proxy( function() {
								this.clearPaste($tmpel);
								if( intoCode ) {
									$tmpel.find('a').replaceWith( function() {
										return $(this).contents();
									} );
								}
								var html = $tmpel.html().replace(/&nbsp;/g, "\u00A0");
								$tmpel.remove();
								this.$body.attr('contentEditable', 'true');
								this.body.focus();
								this.insertAtCursor(html, false);
								this.lastRange = false;
							}, this ), 1 );
						}
					}, this ) );
					if( ! $.browser.opera && ! $.browser.msie ) {
						$(this.doc).bind('keydown', $.proxy( function(e) {
							if(e.which == 13) {
								var isLi = this.isContain( this.getSelectNode(), 'li' );
								if( ! isLi ) {
									if(e.preventDefault) {
										e.preventDefault();
									}
									if( $.browser.msie ) {
										var range = this.doc.selection.createRange();
										range.pasteHTML('<span><br></span>');
										range.collapse(false);
										range.select();
									} else {
										this.checkForLastBR( this.getSelectNode() );
										this.insertAtCursor('<br>', false);
									}
								}
							}
						}, this ) );
					}
					if( this.options.tabInsert === true ) {
						this.$body.bind( 'keydown', $.proxy( function(e) {
							if( e.which == 9 ) {
								this.insertAtCursor("<span>&nbsp;&nbsp;&nbsp;&nbsp;</span>");
								return false;
							}
						}, this ) );
					}
					$(this.doc).bind( 'keyup', $.proxy( function(e) {
						this.$iFrame.contents().find('legend br').remove();
					}, this ) );
					$(this.doc).bind( 'mouseup keyup', $.proxy(this.updateUI, this) );
					$(this.doc).bind( 'mousedown', $.proxy( function(e) {
						this.checkForLastBR(e.target);
					}, this ) );
					if( this.options.hotkeys === true ) {
						$(this.doc).bind( 'keydown', $.proxy(this.presskey, this) );
					}
					if( ! $.browser.msie && ! this.isMobile && ! this.isTouch() ) {
						var maxHeight = $(window).height();
						maxHeight = maxHeight - maxHeight / 4;
						this.$body.keypress( {
							'body': this.$body,
							'iframe': this.$iFrame
						}, function(e) {
							var height = e.data.body.outerHeight(true);
							if( height < maxHeight && height > e.data.iframe.height() ) {
								e.data.iframe.animate( { 'height': height + 32 }, 100 );
							}
						} );
					};
				}, this ) ).insertAfter(this.$txtArea);
			}
			this.$txtArea.bind( 'mouseup keyup', $.proxy(this.updateUI, this) );
			if( this.options.hotkeys === true ) {
				$(document).bind( 'keydown', $.proxy(this.presskey, this) );
			}
		},
		isTouch: function() {
			return /android.+mobile|avantgo|bada\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|iris|kindle|lge |maemo|midp|mmp|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|symbian|treo|up\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test( navigator.userAgent || navigator.vendor || window.opera ) || /1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\-(n|u)|c55\/|capi|ccwa|cdm\-|cell|chtm|cldc|cmd\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\-s|devi|dica|dmob|do(c|p)o|ds(12|\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\-|_)|g1 u|g560|gene|gf\-5|g\-mo|go(\.w|od)|gr(ad|un)|haie|hcit|hd\-(m|p|t)|hei\-|hi(pt|ta)|hp( i|ip)|hs\-c|ht(c(\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\-(20|go|ma)|i230|iac( |\-|\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\/)|klon|kpt |kwc\-|kyo(c|k)|le(no|xi)|lg( g|\/(k|l|u)|50|54|e\-|e\/|\-[a-w])|libw|lynx|m1\-w|m3ga|m50\/|ma(te|ui|xo)|mc(01|21|ca)|m\-cr|me(di|rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\-2|po(ck|rt|se)|prox|psio|pt\-g|qa\-a|qc(07|12|21|32|60|\-[2-7]|i\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\-|oo|p\-)|sdk\/|se(c(\-|0|1)|47|mc|nd|ri)|sgh\-|shar|sie(\-|m)|sk\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\-|v\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\-|tdg\-|tel(i|m)|tim\-|t\-mo|to(pl|sh)|ts(70|m\-|m3|m5)|tx\-9|up(\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|xda(\-|2|g)|yas\-|your|zeto|zte\-/i.test( ( navigator.userAgent || navigator.vendor || window.opera ).substr(0, 4) ) || navigator.userAgent.match(/iPad/i) != null;
		},
		buildToolbar: function() {
			if( this.options.toolbar === false ) {
				return false;
			}
			this.$toolbar = $('<div/>').addClass('wysibb-toolbar').prependTo(this.$editor);
			if( $.browser.msie ) {
				var ieVersion = parseInt( $.browser.version );
				this.$toolbar.addClass( 'wysibb-toolbar-ie wysibb-toolbar-ie-' + ieVersion );
				if( ieVersion != 10 ) {
					this.$toolbar.addClass('wysibb-toolbar-ie-not-10');
				}
			}
			var $btnContainer;
			$.each( this.options.buttons, $.proxy( function(i, bn) {
				var opt = this.options.allButtons[bn];
				if( i == 0 || bn == '|' || bn == '-' ) {
					if( bn == '-' ) {
						this.$toolbar.append('<div/>');
					}
					$btnContainer = $('<div class="wysibb-toolbar-container' + ( $.browser.msie ? ' wysibb-toolbar-container-ie' : '' ) + '"></div>').appendTo(this.$toolbar);
				}
				if(opt) {
					if( opt.type == 'colorpicker' ) {
						this.buildColorpicker($btnContainer, bn, opt);
					} else if( opt.type == 'table' ) {
						this.buildTablepicker($btnContainer, bn, opt);
					} else if( opt.type == 'select' ) {
						this.buildSelect($btnContainer, bn, opt);
					} else if( opt.type == 'smilebox' ) {
						this.buildSmilebox($btnContainer, bn, opt);
					} else {
						this.buildButton($btnContainer, bn, opt);
					}
				}
			}, this ) );
			this.$toolbar.find('.btn-tooltip').hover( function () {
				$(this).parent().css("overflow","hidden");
			}, function() {
				$(this).parent().css("overflow","visible");
			} );
			var $bbsw = $( document.createElement('div') ).addClass('wysibb-toolbar-container modeSwitch').html('<div class="wysibb-toolbar-btn" unselectable="on"><span class="btn-inner ve-tlb-bbcode" unselectable="on"></span><span class="btn-tooltip">' + CURLANG.mode_switch_button + '<ins></ins><span></div>').appendTo(this.$toolbar);
			$bbsw.children('.wysibb-toolbar-btn').click( $.proxy( function(e) {
				$(e.currentTarget).toggleClass('on');
				this.modeSwitch();
			}, this ) );
			if($.browser.msie) {
				this.$toolbar.find('*').attr('unselectable', 'on');
			}
			this.$toolbar.find('.wysibb-toolbar-container:empty').remove();
			if( $.browser.msie ) {
				this.$toolbar.find('.wysibb-toolbar-btn').hover( function() {
					$(this).toggleClass('wysibb-toolbar-btn-hovered');
				} );
				this.$toolbar.find('.wbb-dropdown').addClass('wbb-dropdown-ie');
				this.$toolbar.find('.wbb-select').addClass('wbb-select-ie');
			}
		},
		buildButton: function(container, bn, opt) {
			if( typeof container != 'object' ) {
				container = this.$toolbar;
			}
			var btnHTML = (opt.buttonHTML) ? $( this.strf(opt.buttonHTML, this.options) ).addClass('btn-inner') : this.strf('<span class="btn-inner btn-text">{text}</span>', { text: opt.buttonText.replace(/</g,"&lt;") } );
			var hotkey = ( this.options.hotkeys === true && this.options.showHotkeys === true && opt.hotkey ) ? (' <span class="tthotkey">[' + opt.hotkey + ']</span>') : '';
			var $btn = this.isTouch() ? $('<span class="wysibb-toolbar-btn wbb-' + bn + '"></span>').appendTo(container).append(btnHTML) : $('<span class="wysibb-toolbar-btn wbb-' + bn + '"></span>').appendTo(container).append(btnHTML).append( this.strf('<span class="btn-tooltip">{title}<ins/>{hotkey}</span>', { title: opt.title, hotkey: hotkey } ) );
			this.controllers.push($btn);
			$btn.bind( 'queryState', $.proxy( $.browser.msie ? function (e) {
				( this.queryState(bn) ) ? $(e.currentTarget).addClass('on').addClass('on-ie') : $(e.currentTarget).removeClass('on').removeClass('on-ie');
			} : function(e) {
				( this.queryState(bn) ) ? $(e.currentTarget).addClass('on') : $(e.currentTarget).removeClass('on');
			}, this ) );
			$btn.click( $.proxy( function(e) {
				this.execCommand( bn, opt.exvalue || false );
				$(e.currentTarget).trigger('queryState');
			}, this ) );
			$btn.mousedown( function(e) { 
				if(e.preventDefault) e.preventDefault();
			} );
		},
		buildColorpicker: function(container,bn,opt) {
			var $btn = this.isTouch() ? $('<span class="wysibb-toolbar-btn wbb-dropdown wbb-cp"></span>').appendTo(container).append('<div class="ve-tlb-colorpick"><span class="cp-line"></span></div><ins class="ar"/>') : $('<span class="wysibb-toolbar-btn wbb-dropdown wbb-cp"></span>').appendTo(container).append('<div class="ve-tlb-colorpick"><span class="cp-line"></span></div><ins class="ar"/>').append( this.strf('<span class="btn-tooltip">{title}<ins/></span>', { title: opt.title } ) );
			var $cpline = $btn.find('.cp-line');
			var $dropblock = $('<div class="wbb-list"></div>').appendTo($btn); 
			$dropblock.append('<div class="nc">' + CURLANG.auto + '</div>');
			var colorlist = (opt.colors) ? opt.colors.split(',') : []; 
			for( var j = 0; j < colorlist.length; j++ ) {
				colorlist[j] = $.trim( colorlist[j] );
				if( colorlist[j] == '-') { 
					$dropblock.append('<span class="pl"></span>');
				} else { 
					$dropblock.append( this.strf('<div class="sc" style="background:{color}" title="{color}" uwbb-color="{color}"></div>', { color: colorlist[j] } ) );
				}
			}
			var basecolor = $(document.body).css('color');
			this.controllers.push($btn);
			$btn.bind( 'queryState', $.proxy( function(e) {
				$cpline.css('background-color', basecolor);
				var r = this.queryState('fontcolor', true);
				if(r) {
					$cpline.css( 'background-color', (this.options.bbmode) ? r.color : r );
				}
			}, this ) );
			$btn.click( $.proxy( function(e) {
				this.dropdownclick('.wbb-cp', '.wbb-list', e);
			}, this ) );
			$btn.find('.sc').click( $.proxy( function(e) {
				var c = $(e.currentTarget).attr('uwbb-color');
				this.execCommand('fontcolor', c);
				$btn.trigger('queryState');
				if( $.browser.msie ) {
					this.lastRange=false;
				}
			}, this ) );
			$btn.find('.nc').click( $.proxy( function(e) {
				this.execCommand('fontcolor', basecolor);
				$btn.trigger('queryState');
			}, this ) );
			$btn.mousedown( function(e) { 
				if(e.preventDefault) e.preventDefault();
			} );
		},
		buildTablepicker: function(container, bn, opt) {
			var $btn = $('<div class="wysibb-toolbar-btn wbb-dropdown wbb-tbl"></div>').appendTo(container).append('<span class="btn-inner ve-tlb-table"></span><ins class="ar"/>').append( this.strf('<span class="btn-tooltip">{title}<ins/></span>', { title: opt.title } ) );
			var $dropblock = $('<div class="wbb-list"></div>').appendTo($btn);
			var rows = opt.rows || 10;
			var cols = opt.cols || 10;
			var allcount = rows * cols;
			$dropblock.css( 'width', ( cols * opt.cellwidth + 2 ) + 'px').css('height', ( rows * opt.cellwidth + 2 ) + 'px' );
			for( var j = 1; j <= cols; j++ ) {
				for(var h = 1; h <= rows; h++ ) {
					var html = '<div class="tbl-sel" style="width:' + ( j * opt.cellwidth ) + 'px;height:' + ( h * opt.cellwidth ) + 'px;z-index:' + ( --allcount ) + '" title="' + h + ',' + j + '"></div>';
					$dropblock.append(html);
				}
			}
			$btn.find('.tbl-sel').click( $.proxy( function(e) {
				var t = $(e.currentTarget).attr('title');
				var rc = t.split(',');
				var code = this.options.bbmode ? '[table]' : '<table class="wbb-table" cellspacing="5" cellpadding="0">';
				for( var i = 1; i <= rc[0]; i++ ) {
					code += this.options.bbmode ? ' [tr]\n' : '<tr>';
					for( var j = 1; j <= rc[1]; j++ ) {
						code += this.options.bbmode ? '  [td][/td]\n' : '<td>\uFEFF</td>';
					}
					code += this.options.bbmode ? '[/tr]\n' : '</tr>';
				}
				code += this.options.bbmode ? '[/table]' : '</table>';
				this.insertAtCursor(code);
			}, this ) );
			$btn.click( $.proxy( function(e) {
				this.dropdownclick('.wbb-tbl', '.wbb-list', e);
			}, this ) );
		},
		buildSelect: function(container, bn, opt) {
			var $btn = this.isTouch() ? $('<div class="wysibb-toolbar-btn wbb-select wbb-' + bn + '"></div>').appendTo(container).append( this.strf('<span class="val">{' + ( opt.buttonHTML ? 'buttonhtml' : 'title' ) + '}</span><ins class="sar"/>', opt) ) : $('<div class="wysibb-toolbar-btn wbb-select wbb-' + bn + '"></div>').appendTo(container).append( this.strf('<span class="val">{' + ( opt.buttonHTML ? 'buttonhtml' : 'title' ) + '}</span><ins class="sar"/>', opt) ).append( this.strf( '<span class="btn-tooltip">{title}<ins/></span>', { title: opt.title } ) );
			var $sblock = $('<div class="wbb-list"></div>').appendTo($btn);
			var $sval = $btn.find('span.val');
			var olist = ( $.isArray(opt.options) ) ? opt.options : opt.options.split(',');
			for( var i = 0; i < olist.length; i++ ) {
				var oname = olist[i];
				if( typeof oname == 'string' ) {
					var option = this.options.allButtons[oname];
					if(option) {
						if( option.html ) {
							$('<span/>').addClass('option').attr('oid', oname).attr('cmdvalue', option.exvalue).appendTo($sblock).append( this.strf( option.html, {seltext: option.title } ) );
						} else {
							$sblock.append( this.strf('<span class="option" oid="' + oname + '" cmdvalue="' + option.exvalue + '">{title}</span>', option) );
						}
					}
				} else {
					var params = {
						seltext: oname.title
					}
					params[ opt.valueBBname ] = oname.exvalue;
					$('<span/>').addClass('option').attr('oid',bn).attr('cmdvalue', oname.exvalue).appendTo($sblock).append( this.strf(opt.html, params) );
				}
			}
			this.controllers.push($btn);
			$btn.bind( 'queryState', $.proxy( function(e) {
				$sval.html( opt.buttonHTML || opt.title );
				$btn.find('.option.selected').removeClass('selected');
				$btn.find('.option').each( $.proxy( function(i, el) {
					var $el = $(el);
					var r = this.queryState( $el.attr("oid"), true );
					var cmdvalue = $el.attr('cmdvalue');
					if( ( cmdvalue && r == $el.attr("cmdvalue") ) || ( ! cmdvalue && r ) ) {
						$sval.text( $el.text().replace(/pt$/, '') );
						$el.addClass('selected');
						return false;
					}
				}, this ) );
			}, this ) );
			$btn.click( $.proxy( function(e) {
				this.dropdownclick('.wbb-select', '.wbb-list', e);
			}, this ) );
			$btn.find('.option').click( $.proxy( function(e) {
				var oid = $(e.currentTarget).attr('oid');
				var cmdvalue = $(e.currentTarget).attr('cmdvalue');
				var opt = this.options.allButtons[oid];
				this.execCommand(oid, opt.exvalue || cmdvalue || false);
				$(e.currentTarget).trigger('queryState');
				if(this.lastRange) {
					this.lastRange = false
				}
			}, this ) );
		},
		buildSmilebox: function(container, bn, opt) {
			if( $.isArray( this.options.smileList ) && this.options.smileList.length ) {
				var $btnHTML = $( this.strf(opt.buttonHTML, opt) ).addClass('btn-inner');
				var $btn = this.isTouch() ? $('<div class="wysibb-toolbar-btn wbb-smilebox wbb-' + bn + '">').appendTo(container).append($btnHTML) : $('<div class="wysibb-toolbar-btn wbb-smilebox wbb-' + bn + '">').appendTo(container).append($btnHTML).append( this.strf( '<span class="btn-tooltip">{title}<ins/></span>', { title: opt.title } ) );
				var $sblock = $('<div class="wbb-list"></div>').appendTo($btn);
				if( $.isArray(this.options.smileList) ) {
					$.each( this.options.smileList, $.proxy( function(i, sm) {
						$('<span/>').addClass('smile').appendTo($sblock).append( $( this.strf(sm.img, this.options) ).attr('title', sm.title) );
					}, this ) );
				}
				$btn.click( $.proxy( function(e) {
					this.dropdownclick('.wbb-smilebox', '.wbb-list', e);
				}, this ) );
				$btn.find('.smile').click( $.proxy( function(e) {
					this.insertAtCursor( this.options.bbmode ? ' ' + this.toBB(e.currentTarget) + ' ' : ' ' + $(e.currentTarget).html() + '&nbsp;' );
				}, this ) );
			}
		},
		updateUI: function(e) {
			if( ( e.which >= 8 && e.which <= 46 ) || e.which > 90 || e.type == 'mouseup' ) {
				$.each( this.controllers, $.proxy( function(i, $btn) {
					$btn.trigger('queryState');
				}, this ) );
			}
			if( this.options.autoresize === true ) {
				this.autoresize();
			}
		},
		initModal: function() {
			this.$modal = $('<div/>').attr('id', 'wbbmodal').prependTo(document.body).html('<div class="wbbm"><div class="wbbm-title"><span class="wbbm-title-text"></span><span class="wbbclose" title="' + CURLANG.close + '">×</span></div><div class="wbbm-content"></div><div class="wbbm-bottom"><button id="wbbm-submit" class="wbb-button">' + CURLANG.save + '</button><button id="wbbm-cancel" class="wbb-cancel-button">' + CURLANG.cancel + '</button><button id="wbbm-remove" class="wbb-remove-button">' + CURLANG.remove + '</button></div></div>').hide();
			this.$modal.find('#wbbm-cancel, .wbbclose').click( $.proxy(this.closeModal, this) );
			this.$modal.bind( 'click', $.proxy( function(e) {
				if( $(e.target).parents('.wbbm').size() == 0 ) {
					this.closeModal();
				}
			}, this ) );
			$(document).bind('keydown', $.proxy(this.escModal, this) );
			if( this.options.onlyBBmode !== true ) {
				$(this.doc).bind('keyup', $.proxy(this.escModal, this) );
			}
		},
		initHotkeys: function() {
			this.hotkeys=[];
			var klist = "0123456789       abcdefghijklmnopqrstuvwxyz";
			$.each( this.options.allButtons, $.proxy( function(cmd, opt) {
				if(opt.hotkey) {
					var keys = opt.hotkey.split('+');
 					if( keys && keys.length >= 2 ) {
						var metasum = 0;
						var key = keys.pop();
						$.each( keys,function(i, k) {
							switch( $.trim( k.toLowerCase() ) ) {
								case 'ctrl': {
									metasum += 1;
									break;
								}
								case 'shift': {
									metasum += 4; break;
								}
								case 'alt': {
									metasum += 7;
									break;
								}
							}
						})
						if( metasum > 0 ) {
							if( ! this.hotkeys[ 'm' + metasum ] ) {
								this.hotkeys[ 'm' + metasum ] = [];
							}
							this.hotkeys[ 'm' + metasum ][ 'k' + ( klist.indexOf(key) + 48 ) ] = cmd;
						}
					}
				}
			}, this ) )
		},
		presskey: function(e) {
			if( e.ctrlKey == true || e.shiftKey == true || e.altKey == true ) {
				var metasum = ( ( e.ctrlKey == true ) ? 1 : 0 ) + ( ( e.shiftKey == true ) ? 4 : 0 ) + ( ( e.altKey == true ) ? 7 : 0 );
				if( this.hotkeys[ 'm' + metasum ] && this.hotkeys[ 'm' + metasum ][ 'k' + e.which ] ) {
					this.execCommand( this.hotkeys[ 'm' + metasum ][ 'k' + e.which ], false );
					e.preventDefault();
					return false;
				}
			}
		},
		execCommand: function(command, value) {
			var opt = this.options.allButtons[command];
			var queryState = this.queryState(command, value);
			if( opt.excmd ) {
				if( this.options.bbmode ) {
					if( queryState && opt.subInsert != true ) {
						this.wbbRemoveCallback(command, value);
					} else {
						var v = {};
						if( opt.valueBBname && value ) {
							v[opt.valueBBname] = value;
						}
						this.insertAtCursor( this.getBBCodeByCommand(command, v) );
					}
				} else {
					this.execNativeCommand(opt.excmd, value || false);
				}
			} else if( ! opt.cmd ) {
				this.wbbExecCommand.call(this, command, value, queryState);
			} else {
				opt.cmd.call(this, command, value, queryState);
			}
		},
		queryState: function(command, withvalue) {
			var opt = this.options.allButtons[command];
			if(this.options.bbmode) {
				for( var i = 0; i < opt.bbSelector.length; i++ ) {
					var b = this.isBBContain( opt.bbSelector[i] );
					if(b) {
						return this.getParams(b, opt.bbSelector[i], b[1]);
					}
				}
				return false;
			} else {
				if(opt.excmd) {
					if(withvalue) {
						var v = ( this.doc.queryCommandValue(opt.excmd) + '').replace(/\'/g, '');
						if( opt.excmd == 'foreColor' ) {
							v = this.rgbToHex(v);
						}
						return v;
					} else {
						return this.doc.queryCommandState(opt.excmd);
					}
				} else {
					for( var i = 0; i < opt.rootSelector.length; i++ ) {
						var n = this.isContain( this.getSelectNode(), opt.rootSelector[i] );
						if(n) {
							return this.getParams( n, opt.rootSelector[i] );
						}
					}
					return false;
				}
			}
		},
		wbbExecCommand: function(command, value, queryState) {
			var opt = this.options.allButtons[command];
			if( opt.modal ) {
				if( $.isFunction(opt.modal) ) {
					opt.modal.call(this, command, opt.modal, queryState);
				} else {
					this.showModal.call(this, command, opt.modal, queryState);
				}
			} else {
				if( queryState && opt.subInsert != true ) {
					this.wbbRemoveCallback(command);
				} else {
					this.wbbInsertCallback(command, value)
				}
			}
		},
		wbbInsertCallback: function(command, paramobj) {
			if( typeof paramobj != 'object' ) {
				paramobj = {};
			}
			var data = this.getCodeByCommand(command, paramobj);
			this.insertAtCursor(data);
			var snode = this.getSelectNode();
			if( snode.nodeType == 3 ) {
				snode = snode.parentNode;
			}
			if( $(snode).is('span, font') ) {
				this.selectNode(snode);
			}
		},
		wbbRemoveCallback: function(command, clear) {
			var opt = this.options.allButtons[command];
			if( this.options.bbmode ) {
				var pos = this.getCursorPosBB();
				var stextnum = 0;
				$.each( opt.bbSelector, $.proxy( function(i, bbcode) {
					var stext = bbcode.match(/\{[\s\S]+?\}/g);
					$.each( stext, function(n, s) {
						if( s.toLowerCase() == "{seltext}" ) {
							stextnum = n;
							return false;
						}
					} );
					var a = this.isBBContain(bbcode);
					if(a) {
						this.txtArea.value = this.txtArea.value.substr( 0, a[1] ) + this.txtArea.value.substr( a[1], this.txtArea.value.length - a[1] ).replace( a[0][0], clear === true ? '' : a[0][ stextnum + 1 ] );
						this.setCursorPosBB( a[1] );
						return false;
					}
				}, this ) );
			} else {
				var node = this.getSelectNode();
				$.each( opt.rootSelector, $.proxy( function(i, s) {
					var root = this.isContain(node, s);
					var $root = $(root);
					var cs = this.options.rules[s][0][1];
					if( $root.is('span[wbb]') || ! $root.is('span, font') ) {
						if( clear === true ) {
							$root.remove();
						} else {
							if( cs && cs['seltext'] && cs['seltext']['sel'] ) {
								$root.replaceWith( $root.find( cs['seltext']['sel'] ).html() );
							} else {
								$root.replaceWith( $root.html() );
							}
						}
						return false;
					} else {
						var rng = this.getRange();
						var shtml = this.getSelectText();
						var rnode = this.getSelectNode();
						if( shtml == '' ) {
							shtml = '\uFEFF';
						} else {
							shtml = this.clearFromSubInsert(shtml, command);
						}
						var ins = this.elFromString(shtml);
						var before_rng = window.getSelection ? rng.cloneRange() : this.body.createTextRange();
						var after_rng = window.getSelection ? rng.cloneRange() : this.body.createTextRange();
						if( window.getSelection ) {
							this.insertAtCursor('<span id="wbbdivide">' + shtml + '</span>');
							var div = $root.find('span#wbbdivide').get(0);
							before_rng.setStart(root.firstChild, 0);
							before_rng.setEndBefore(div);
							after_rng.setStartAfter(div);
							after_rng.setEndAfter(root.lastChild);
						} else {
							before_rng.moveToElementText(root);
							after_rng.moveToElementText(root);
							before_rng.setEndPoint('EndToStart', rng);
							after_rng.setEndPoint('StartToEnd', rng);
						}
						var bf = this.getSelectText(false, before_rng);
						var af = this.getSelectText(false, after_rng);
						if( af != '' ) {
							var $af = $root.clone().html(af);
							$root.after($af);
						}
						if( clear !== true ) {
							$root.after(ins);
						}
						if( window.getSelection ) {
							$root.html(bf);
							if( clear !== true ) {
								this.selectNode(ins);
							}
						} else {
							$root.replaceWith(bf);
						}
						return false;
					}
				}, this ) );
			}
		},
		execNativeCommand: function(cmd, param) {
			if( ! this.lastRange ) {
				this.lastRange = this.getRange();
			}
			this.body.focus();
			if( cmd == 'insertHTML' && ( ! window.getSelection || $.browser.msie ) ) {
				var r = this.lastRange ? this.lastRange : this.doc.selection.createRange();
				if( ( ! document.doctype || document.compatMode == 'BackCompat' ) && ( param.indexOf('class="sm"') !== -1 ) ) {
					r = this.doc.selection.createRange();
				}
				r.pasteHTML(param);
				var txt = $('<div/>').html(param).text();
				var brsp = txt.indexOf("\uFEFF");
				if( brsp > -1 ) {
					r.moveStart( 'character', (-1) * ( txt.length - brsp ) );
					r.select();
				}
				this.lastRange = false;
			} else if( cmd == 'insertHTML' ) {
				var sel = this.getSelection();
				var e = this.elFromString(param);
				var rng = this.lastRange ? this.lastRange : this.getRange();
				rng.deleteContents();
				rng.insertNode(e);
				if( $.browser.opera && $(e).contents().length <= 1 ) {
					rng.selectNode(e);
				} else {
					rng.collapse(false);
				}
				sel.removeAllRanges();
				sel.addRange(rng);
				this.lastRange = false;
			} else {
				if( typeof param == 'undefined') {
					param = false;
				}
				if( ! $.browser.opera && this.lastRange ) {
					this.selectRange(this.lastRange);
					this.lastRange = false;
				}
				this.doc.execCommand(cmd, false, param);
			}
		},
		getCodeByCommand: function(command, paramobj) {
			return (this.options.bbmode) ? this.getBBCodeByCommand(command, paramobj) : this.getHTMLByCommand(command, paramobj);
		},
		getBBCodeByCommand: function(command, params) {
			if( ! this.options.allButtons[command] ) {
				return '';
			}
			if( typeof params == 'undefined') {
				params = {};
			}
			params = this.keysToLower(params);
			if( ! params['seltext'] ) {
				params['seltext'] = this.getSelectText(true);
			}
			var bbcode = this.options.allButtons[command].bbcode;
			bbcode = this.strf(bbcode, params);
			var rbbcode = null;
			var tr = [];
			$.each( this.options.allButtons[command].transform, function(html, bb) {
				tr.push(bb);
			} );
			tr = this.sortArray(tr, -1);
			$.each( tr, function(i, v) {
				var valid = true;
				v = v.replace(/\{\S+?\}/g, function(a) {
					a = a.substr( 1, a.length - 2 ); 
					var r = params[ a.toLowerCase() ];
					if( ! r ) {
						valid = false;
					};
					return r;
				} );
				if(valid) {
					rbbcode = v;
					return false;
				}
			} );
			return rbbcode || bbcode;
		},
		getHTMLByCommand: function(command, params) {
			if( ! this.options.allButtons[command] ) {
				return '';
			}
			params = this.keysToLower(params);
			if( typeof params == 'undefined' ) {
				params = {};
			}
			if( ! params['seltext'] ) {
				params['seltext'] = this.getSelectText(false);
				if( params['seltext'] == '') {
					params['seltext'] = "\uFEFF";
				} else {
					params['seltext'] = this.clearFromSubInsert(params['seltext'], command);
				}
			}
			var html = this.options.allButtons[command].html;
			html = this.strf(html, params);
			var rhtml = null;
			var tr = [];
			$.each( this.options.allButtons[command].transform, function(html, bb) {
				tr.push(html);
			} );
			tr = this.sortArray(tr, -1);
			$.each( tr, function(i, v) {
				var valid=true;
				v = v.replace(/\{\S+\}/g, function(a) {
					a = a.substr( 1, a.length - 2 ); 
					var r = params[ a.toLowerCase() ];
					if( ! r ) {
						valid = false;
					}
					return r;
				} );
				if(valid) {
					rhtml = v;
					return false;
				}
			} );
			return rhtml || html;
		},
		getSelection: function() {
			if( window.getSelection && ! $.browser.msie ) {
				return this.options.bbmode ? window.getSelection() : this.framewindow.getSelection();
			} else if(document.selection) {
				return this.options.bbmode ? document.selection.createRange() : this.doc.selection.createRange();
			}
		},
		getSelectText: function(fromTxtArea, range) {
			if(fromTxtArea) {
				if( ! $.browser.opera ) {
					this.txtArea.focus();
				}
				if( 'selectionStart' in this.txtArea ) {
					var l = this.txtArea.selectionEnd - this.txtArea.selectionStart;
					return this.txtArea.value.substr(this.txtArea.selectionStart, l);
				} else {
					var r = document.selection.createRange();
					return r.text;
				}
			} else {
				if( ! $.browser.opera ) {
					this.body.focus();
				}
				if( ! range ) {
					range=this.getRange();
				}
				if( this.framewindow.getSelection && ! $.browser.msie ) {
					if(range) {
						return $('<div/>').append( range.cloneContents() ).html();
					}
				} else {
					return range.htmlText;
				}
			}
			return '';
		},
		getRange: function() {
			if( window.getSelection && ! $.browser.msie ) {
				var sel = this.getSelection();
				if( sel.getRangeAt && sel.rangeCount > 0 ) {
					return sel.getRangeAt(0);
				} else if(sel.anchorNode) {
					var range = this.options.bbmode ? document.createRange() : this.doc.createRange();
					range.setStart(sel.anchorNode, sel.anchorOffset);
					range.setEnd(sel.focusNode, sel.focusOffset);
					return range;
				}
			} else {
				return this.options.bbmode === true ? document.selection.createRange() : this.doc.selection.createRange();
			}
		},
		insertAtCursor: function(code, forceBBMode) {
			if( typeof code != 'string' ) {
				code = $("<div/>").append(code).html();
			}
			if( ( this.options.bbmode && typeof forceBBMode == 'undefined') || forceBBMode === true ) {
				var clbb = code.replace(/.*(\[\/\S+?\])$/, "$1");
				var p = this.getCursorPosBB() + code.indexOf(clbb);
				if(document.selection) {
					if( ! $.browser.opera ) {
						this.txtArea.focus();
					}
					this.getSelection().text = code;
				} else if( this.txtArea.selectionStart || this.txtArea.selectionStart == '0') {
					this.txtArea.value = this.txtArea.value.substring(0, this.txtArea.selectionStart) + code + this.txtArea.value.substring(this.txtArea.selectionEnd, this.txtArea.value.length);
				}
				if( p < 0 ) {
					p = 0;
				}
				this.setCursorPosBB(p);
			} else {
				this.execNativeCommand('insertHTML', code);
				var node = this.getSelectNode();
				if( ! $(node).closest('table, tr, td') ) {
					this.splitPrevNext(node);
				}
			}
		},
		getSelectNode: function(rng) {
			if( ! $.browser.opera ) {
				this.body.focus();
			}
			if( ! rng ) {
				rng = this.getRange();
			}
			return ( window.getSelection && ! $.browser.msie ) ? rng.commonAncestorContainer : rng.parentElement();
		},
		getCursorPosBB: function() {
			var pos = 0;
			if( 'selectionStart' in this.txtArea ) {
				pos = this.txtArea.selectionStart;
			} else {
				if( ! $.browser.opera ) {
					this.txtArea.focus();
				}
				var r = this.getRange();
				var rt = document.body.createTextRange();
				rt.moveToElementText(this.txtArea);
				rt.setEndPoint('EndToStart', r);
				pos = rt.text.length;
			}
			return pos;
		},
		setCursorPosBB: function(pos) {
			if( this.options.bbmode ) {
				if( window.getSelection ) {
					this.txtArea.selectionStart = pos;
					this.txtArea.selectionEnd = pos;
				} else {
					var range = this.txtArea.createTextRange();
					range.collapse(true);
					range.move('character', pos);
					range.select();
				}
			}
		},
		selectNode: function(node, rng) {
			if( ! rng ) {
				rng = this.getRange();
			}
			if( window.getSelection ) {
				var sel = this.getSelection();
				rng.selectNodeContents(node)
				sel.removeAllRanges();
				sel.addRange(rng);
			} else {
				rng.moveToElementText(node);
				rng.select();
			}
		},
		selectRange: function(rng) {
			if( ! window.getSelection || $.browser.msie ) {
				this.lastRange.select();
			} else {
				var sel = this.getSelection();
				sel.removeAllRanges();
				sel.addRange(this.lastRange);
			}
		},
		filterByNode: function(node) {
			var $n = $(node);
			var tagName = $n.get(0).tagName.toLowerCase();
			var filter = tagName;
			var attributes = this.getAttributeList( $n.get(0) );
			$.each( attributes, $.proxy( function(i, item) {
				var v = $n.attr(item);
				if( v && ! v.match(/\{.*?\}/) ) {
					if( item == 'style' ) {
						var v = $n.attr(item);
						var va = v.split(';');
						$.each( va, function(i, f) {
							if( f && f.length > 0 ) {
								filter += '[' + item + '*="' + $.trim(f) + '"]';
							}
						} );
					} else {
						filter += '[' + item + '="' + v + '"]';
					}
				} else if( v && item == 'style' ) {
					var vf = v.substr( 0, v.indexOf('{') );
					if( vf && vf != '' ) {
						var v = v.substr( 0, v.indexOf('{') );
						var va = v.split(';');
						$.each( va, function(i, f) {
							filter += '[' + item + '*="' + f + '"]';
						} );
					}
				}
			}, this ) );
			var idx = $n.parent().children(filter).index($n);
			if( idx > 0 ) {
				filter += ':eq(' + $n.index() + ')';
			}
			return filter;
		},
		relFilterByNode: function(node, stop) {
			var p = '';
			while( node && node.tagName != 'BODY' && ! $(node).is(stop) ) {
				p = this.filterByNode(node) + ' ' + p;
				if(node) {
					node = node.parentNode;
				}
			}
			return p;
		},
		getRegexpReplace: function(str, validname) {
			str = str.replace(/(\(|\)|\[|\]|\.|\*|\?|\:|\\)/g,"\\$1") 
				.replace(/\s+/g, "\\s+")
				.replace(validname, "(.+)")
				.replace(/\{\S+\}/g, ".*");
			return str;
		},
		getBBCode: function() {
			if( ! this.options.rules ) {
				return this.$txtArea.val();
			}
			if( this.options.bbmode ) {
				return this.$txtArea.val();
			}
			this.clearEmpty();
			return this.toBB( this.$body.html() );
		},
		toBB: function(data) {
			if( ! data ) {
				return '';
			}
			var $e = ( typeof data == 'string' ) ? $('<span/>').html(data) : $(data);
			if( ! $.browser.msie ) {
				$e.find('div, blockquote, p').each( function() {
					if( this.nodeType != 3 && this.lastChild && this.lastChild.tagName == 'BR') {
						$(this.lastChild).remove();
					}
				} );
			} else {
				$e.find('div, blockquote, p').each( function() {
					$(this).append('<br/>');
				} );
			}
			if( ! $.browser.msie ) if( $e.is('div, blockquote, p') && $e[0].nodeType != 3 && $e[0].lastChild && $e[0].lastChild.tagName == 'BR' ) {
				$( $e[0].lastChild ).remove();
			}
			$e.find('ul > br, table > br, tr > br').remove();
			$e.find('font').each( function() {
				var elem = $(this);
				var face = elem.attr('face');
				if( ! face ) {
					return;
				}
				var exec;
				if( exec = /^\s*([a-z]+)/i.exec(face) ) {
					elem.attr('face', exec[1]);
				}
			} );
			$e.find('a').each( function() {
				var elem = $(this);
				var href = elem.attr('href');
				if( ! href ) {
					return;
				}
				href = href.replace( /^\s*view-source:/i, '' );
				elem.attr('href', href);
			} );
			$e.find('.ucoz-forum-post-image').each( function() {
				var image;
				if( image = /ucoz-forum-post-image-(\d+)/.exec(this.className) ) {
					$(this).replaceWith( '$IMAGE' + image[1] + '$' );
				}
			} );
			var outbb = '';
			$.each( this.options.srules, $.proxy( function(s, bb) {
				$e.find(s).replaceWith( bb[0] );
			}, this ) );
			$e.contents().each( $.proxy( function(i, el) {
				var $el = $(el);
				if( el.nodeType === 3 ) {
					outbb += el.data.replace(/\n+/, '').replace(/\t/g, '   ');
				} else {
					if( el.nodeType == 1 && /^<div>/i.test(el.outerHTML) ) $('<br/><br/>').appendTo($el);
					var rpl, processed = false;
					for( var rootsel in this.options.rules ) {
						if( $el.is(rootsel) ) {
							var rlist = this.options.rules[rootsel];
							for( var i = 0; i < rlist.length; i++ ) {
								var bbcode = rlist[i][0];
								var crules = rlist[i][1];
								var skip = false, keepElement = false, keepAttr = false;
								if( ! $el.is('br') ) {
									bbcode = bbcode.replace(/\n/g, '<br>');
								}
								bbcode = bbcode.replace( /\{(\S+?)\}/g ,function(s) {
									s = s.substr(1, s.length - 2);
									var c = crules[ s.toLowerCase() ];
									if(typeof(c)=="undefined") {
										$.log("Error in configuration of BBCode[" + rootsel + "]. Param: {" + s + "} not found in HTML representation.");
										skip = true;
										return s;
									}
									var $cel = c.sel ? $(el).find(c.sel) : $(el);
									if( c.attr && ! $cel.attr(c.attr) ) {
										skip = true;
										return s;
									}
									var cont = c.attr ? $cel.attr(c.attr) : $cel.html();
									if( typeof cont == 'undefined' || cont == null ) {
										skip=true;
										return s;
									}
									var regexp = c.rgx;
									if( regexp && c.attr == 'style' && regexp.substr( regexp.length - 1, 1 ) != ';' ) {
										regexp += ';';
									}
									if( c.attr == 'style' && cont && cont.substr( cont.length - 1, 1 ) != ';') {
										cont += ';';
									}
									var rgx = regexp ? new RegExp(regexp, '') : false;
									if(rgx) {
										if( cont.match(rgx) ) {
											var m = cont.match(rgx);
											if( m && m.length == 2 ) {
												cont = m[1];
											}
										} else {
											cont = '';
										}
									}
									if( c.attr && skip === false ) {
										if( c.attr == 'style' ) {
											keepElement = true;
											var nstyle = '';
											var r = c.rgx.replace(/^\.\*\?/, '').replace(/\.\*$/, '').replace(/;$/, '');
											$( $cel.attr('style').split(';') ).each( function(idx, style) {
												if( style && style != '' ) {
													if( ! style.match(r) ) {
														nstyle += style + ';';
													}
												}
											} );
											if( nstyle == '' ) {
												$cel.removeAttr('style');
											} else {
												$cel.attr('style', nstyle);
											}
										} else if( c.rgx === false ){
											keepElement=true;
											keepAttr=true;
											$cel.removeAttr(c.attr);
										}
									}
									if( $el.is('table, tr, td, font') ) {
										keepElement=true;
									}
									return cont || '';
								} );
								if(skip) {
									continue;
								}
								if( $el.is('img, br, hr') ) {
									outbb += bbcode;
									break;
								} else {
									if(keepElement) {
										if($.browser.msie) {
											$el.empty().append( $('<span/>').html(bbcode) );
										} else {
											$el.empty().html('<span>' + bbcode + '</span>');
										}
									} else {
										if( $el.is('iframe') ) {
											outbb += bbcode;
										} else {
											$el.empty().html(bbcode);
										}
										break;
									}
								}
							}
						}
					}
					if( $el.is('iframe') ) {
						return true;
					}
					outbb += this.toBB($el);
				}
			}, this ) );
			return outbb;
		},
		getHTML: function(bbdata, init, save) {
			if( ! this.options.bbmode && ! init ) {
				if( save ) {
					var body = $('<span/>').html( this.$body.html() );
					body.find('.ucoz-forum-post-image').each( function() {
						var image;
						if( image = /ucoz-forum-post-image-(\d+)/.exec(this.className) ) {
							$(this).replaceWith( '$IMAGE' + image[1] + '$' );
						}
					} );
					return body.html();
				}
				return this.$body.html();
			}
			bbdata = bbdata.replace(/([^\n])\[\*\]/gi, "$1\n[*]").replace(/(\[\*\][^\n]*?)\[\/list\]/gi, "$1\n[/list]").replace(/\[list\]\n/gi, "[list]");
			bbdata = bbdata.replace(/</g,"&lt;").replace(/\{/g,"&#123;").replace(/\}/g,"&#125;");
			bbdata = bbdata.replace(/\[code\](.*?)\[\/code\]/g, function(s) {
				s = s.substr( "[code]".length, s.length - "[code]".length - "[/code]".length ).replace(/\[/g, "&#91;").replace(/\]/g, "&#93;");
				return "[code]" + s + "[/code]";
			} );
			bbdata = bbdata.replace( /\[quote(=[^\]]+?)?\]\[\/quote\]/g, '[quote$1]&nbsp;[/quote]' );
			bbdata = bbdata.replace( /\[spoiler(=[^\]]+?)?\]\[\/spoiler\]/g, '[spoiler$1]&nbsp;[/spoiler]' );
			bbdata = bbdata.replace( /\[hide(=[^\]]+?)?\]\[\/hide\]/g, '[hide$1]&nbsp;[/hide]' );
			bbdata = bbdata.replace( /\[code\]\[\/code\]/g, '[code]&nbsp;[/code]' );
			if( $.browser.msie ) {
				bbdata = bbdata.replace( /([^\n]*)\n/g, '<p>$1</p>' );
			}
			if( ! save ) {
				bbdata = this.uImagesToHTML(bbdata);
			}
			$.each( this.options.smileList, $.proxy( function(i, row) {
				bbdata = bbdata.replace( new RegExp( this.prepareRGX(row.bbcode), "g"), this.strf(row.img, this.options) );
			}, this ) );
			bbdata = bbdata.replace(/\$/g, '&#36;');
			$.each( this.options.btnlist, $.proxy( function(i, b) {
				if( b != '|' && b != '-') {
					var find = true;
					if( ! this.options.allButtons[b] || ! this.options.allButtons[b].transform ) {
						return true;
					}
					$.each( this.options.allButtons[b].transform, $.proxy( function(html, bb) {
						html = html.replace(/\n/g, '');
						var a = [];
						bb = bb.replace(/(\(|\)|\[|\]|\.|\*|\?|\:|\\|\\)/g,"\\$1");
						bb = bb.replace(/\{\S+?\}/gi, function(s) {
							s = s.substr( 1, s.length - 2 );
							a.push(s);
							return "([\\s\\S]*?)";
						} );
						var n = 0, am;
						while( (am = ( new RegExp(bb, "mgi") ).exec(bbdata) ) != null) {
							if(am) {
								var r = {};
								$.each( a, $.proxy( function(i, k) {
									r[k] = am[ i + 1 ];
								}, this ) );
								var nhtml = html;
								nhtml = this.strf(nhtml, r);
								bbdata = bbdata.replace(am[0], nhtml);
							}
						}
					}, this ) );
				}
			}, this ) );
			$.each(this.options.systr,function(html,bb) {
				bb = bb.replace(/(\(|\)|\[|\]|\.|\*|\?|\:|\\|\\)/g,"\\$1")
					.replace(' ',"\\s");
				bbdata = bbdata.replace(new RegExp(bb,"g"),html);
			});
			return bbdata;
		},
		setUID: function(el, attr) {
			var id = 'wbbid_' + ( ++this.lastid );
			$(el).attr(attr || 'id', id);
			return id;
		},
		keysToLower: function(o) {
			$.each( o, function(k, v) {
				if( k != k.toLowerCase() ) {
					delete o[k];
					o[ k.toLowerCase() ] = v;
				}
			} );
			return o;
		},
		strf: function(str, data) {
			data = this.keysToLower( $.extend( {}, data ) );
			return str.replace(/\{([\w\.]*)\}/g, function (str, key) {
				key = key.toLowerCase();
				var keys = key.split("."), value = data[ keys.shift().toLowerCase() ];
				$.each( keys, function () {
					value = value[this];
				} );
				return (value === null || value === undefined) ? '' : value;
			} );
		},
		elFromString: function(str, doc) {
			if( typeof doc == 'undefined' ) {
				doc = this.doc;
			}
			if( str.indexOf('<') != -1 && str.indexOf('>') != -1 ) {
				var wr = doc.createElement('SPAN');
				$(wr).html(str);
				this.setUID(wr, 'wbb');
				return $(wr).contents().size() > 1 ? wr : wr.firstChild;
			} else {
				return doc.createTextNode(str);
			}
		},
		isContain: function(node, sel) {
			while( node && node.tagName != 'BODY' ) {
				if( $(node).is(sel) ) {
					return node;
				}
				if(node) {
					node = node.parentNode;
				} else {
					return null;
				}
			}
		},
		isBBContain: function(bbcode) {
			var pos = this.getCursorPosBB();
			var b = this.prepareRGX(bbcode);
			var bbrgx = new RegExp(b, 'g');
			var a;
			var lastindex = 0;
			while( ( a = bbrgx.exec(this.txtArea.value) ) != null ) {
				var p = this.txtArea.value.indexOf(a[0], lastindex);
				if( pos > p && pos < ( p + a[0].length ) ) {
					return [a, p];
				}
				lastindex = p + 1;
			}
		},
		prepareRGX: function(r) {
			return r.replace(/(\[|\]|\)|\(|\.|\*|\?|\:|\||\\)/g, "\\$1").replace(/\{.*?\}/g, "([\\s\\S]*?)");
		},
		checkForLastBR: function(node) {
			if( ! node ) {
				$node = this.body;
			} 
			if( node.nodeType == 3 ) {
				node=node.parentNode;
			}
			var $node = $(node);
			if( this.options.bbmode === false && $node.is('div, blockquote') && $node.contents().size() > 0 ) {
				var l = $node[0].lastChild;
				if( ! l || ( l && l.tagName != 'BR' ) ) {
					$node.append('<br/>');
				}
			}
			if( this.$body.contents().size() > 0 && this.body.lastChild.tagName != 'BR' ) {
				this.$body.append('<br/>');
			}
		},
		getAttributeList: function(el) {
			var a = [];
			$.each( el.attributes, function(i, attr) {
				if(attr.specified) {
					a.push(attr.name);
				}
			} );
			return a;
		},
		clearFromSubInsert: function(html, cmd) {
			var $wr = $('<div/>').html(html);
			$.each( this.options.allButtons[cmd].rootSelector, $.proxy( function(i, s) {
				var seltext = this.options.rules[s][0][1]['seltext']['sel'];
				var res = true;
				$wr.find('*').each( function() {
					if( $(this).is(s) ) {
						if( seltext && seltext['sel'] ) {
							$(this).replaceWith( $(this).find( seltext['sel'].toLowerCase() ).html() );
						} else {
							$(this).replaceWith( $(this).html() );
						}
						res = false;
					}
				} );
				return res;
			}, this ) );
			return $wr.html();
		},
		splitPrevNext: function(node) {
			if( node.nodeType == 3 ) {
				node = node.parentNode
			};
			var f = this.filterByNode(node).replace(/\:eq.*$/g, '');
			if( $(node.nextSibling).is(f) ) {
				$(node).append( $(node.nextSibling).html() );
				$(node.nextSibling).remove();
			}
			if( $(node.previousSibling).is(f) ) {
				$(node).prepend( $(node.previousSibling).html() );
				$(node.previousSibling).remove();
			}
		},
		modeSwitch: function(noFocus) {
			if(this.options.bbmode) {
				this.$body.html( this.getHTML( this.$txtArea.val() ) );
				this.$txtArea.hide();
				this.$iFrame.show();
				if( ! noFocus ) {
					this.body.focus();
				}
			} else {
				this.$txtArea.val( this.getBBCode() );
				this.$iFrame.hide();
				this.$txtArea.show();
				if( ! noFocus ) {
					this.$txtArea.get(0).focus();
				}
			}
			this.options.bbmode = ! this.options.bbmode;
		},
		clearEmpty: function () {
			this.$body.children().filter(emptyFilter).remove();
			function emptyFilter() {
				if( ! $(this).is('span, font, a') ) {
					return false;
				}
				if( ! $(this).hasClass('wbbtab') && $.trim( $(this).html() ).length == 0 ) {
					return true;
				} else if( $(this).children().size() > 0 ) {
					$(this).children().filter(emptyFilter).remove();
					if( $(this).html().length == 0 && this.tagName != 'BODY') {
						return true;
					}
				}
			}
		},
		dropdownclick: function(bsel, tsel, e) {
			var $btn = $(e.currentTarget).closest(bsel);
			if( $btn.attr('wbbshow') ) {
				$btn.removeAttr('wbbshow');
				$(document).unbind('mousedown', this.dropdownhandler);
				if(this.doc) {
					$(this.doc).unbind('mousedown', this.dropdownhandler);
				}
			} else {
				this.$editor.find('*[wbbshow]').each( function(i, el) {
					$(el).removeClass('on').find($(el).attr('wbbshow')).hide().end().removeAttr('wbbshow');
				} );
				$btn.attr('wbbshow', tsel);
				$(document.body).bind('mousedown', $.proxy( function(evt) {
					this.dropdownhandler($btn, bsel ,tsel, evt);
				}, this ) );
				if(this.$body) {
					this.$body.bind('mousedown', $.proxy( function(evt) {
						this.dropdownhandler($btn, bsel, tsel, evt);
					}, this ) );
				}
			}
			$btn.find(tsel).toggle();
			$btn.toggleClass('on');
		},
		dropdownhandler: function($btn, bsel, tsel, e) {
			if( $(e.target).parents(bsel).size() == 0 ) {
				$btn.removeClass('on').find(tsel).hide();
				$(document).unbind('mousedown', this.dropdownhandler);
				if(this.$body) {
					this.$body.unbind('mousedown', this.dropdownhandler);
				}
			}
		},
		rgbToHex: function(rgb) {
			if( rgb.substr(0, 1) == '#' ) {
				return rgb;
			}
			if( rgb.indexOf("rgb") == -1 ) {
				var color = parseInt(rgb);
				color = ( ( color & 0x0000ff ) << 16 ) | ( color & 0x00ff00 ) | ( ( color & 0xff0000 ) >>> 16 );
				return '#' + color.toString(16);
			}
			var digits = /(.*?)rgb\((\d+),\s*(\d+),\s*(\d+)\)/.exec(rgb);
			return "#" + this.dec2hex( parseInt( digits[2] ) ) + this.dec2hex( parseInt( digits[3] ) ) + this.dec2hex( parseInt( digits[4] ) ); 
		},
		dec2hex: function(d) {
			if( d > 15 ) {
				return d.toString(16);
			} else {
				return '0' + d.toString(16);
			}
		},
		sync: function() {
			if(this.options.bbmode) {
				this.$body.html( this.getHTML(this.txtArea.value, true) );
			} else {
				this.$txtArea.val( this.getBBCode() );
			}
		},
		clearPaste: function(el) {
			var $block = $(el);
			$.each( this.options.rules, $.proxy( function(s, bb) {
				$block.find(s).attr('wbbkeep', 1);
			}, this ) );
			$block.find("*[wbbkeep!='1']").each( $.proxy( function(i, el) {
				var $this = $(el);
				if( $this.is('div, p') && ( $this.children() == 0 || ( el.lastChild && el.lastChild.tagName != 'BR' ) ) ) {
					$this.after('<br/>').after( $this.contents() ).remove();
				} else {
					$this.after( $this.html() ).remove();
				}
			}, this ) );
			$block.find('*[wbbkeep]').removeAttr('wbbkeep').removeAttr('style');
		},
		sortArray: function(ar, asc) {
			ar.sort( function(a, b) {
				return ( a.length - b.length ) * ( asc || 1 );
			} );
			return ar;
		},
		smileFind: function() {
			if(this.options.smilefind) {
				var $smlist = $(this.options.smilefind).find('img[alt]');
				if( $smlist.size() > 0 ) {
					this.options.smileList = [];
					$smlist.each( $.proxy( function(i, el) {
						var $el = $(el);
						this.options.smileList.push( {
							title: $el.attr('title'),
							bbcode: $el.attr('alt'),
							img: $el.removeAttr('alt').removeAttr('title')[0].outerHTML
						} );
					}, this ) );
				}
			}
		},
		destroy: function() {
			this.$editor.replaceWith(this.$txtArea);
			this.$txtArea.removeClass('wysibb-texarea').show();
			this.$modal.remove();
			this.$txtArea.data('wbb', null);
		},
		autoresize: function() {
			clearTimeout(this.resizeTimer);
			this.resizeTimer=setTimeout( $.proxy( function() {
				var wh = this.$iFrame.outerHeight();
				var ih = this.$body.outerHeight();
				if( ih > wh ) {
					if( ih > this.options.resize_maxheight ) {
						ih = this.options.resize_maxheight;
					}
					this.$iFrame.height( ( ih + 30 ) + 'px');
					this.$txtArea.height( ( ih + 30 ) + 'px');
				}
			}, this ), 200 );
		},
		showModal: function(cmd, opt, queryState) {
			var $cont = this.$modal.find('.wbbm-content').html('');
			var $wbbm = this.$modal.find('.wbbm').removeClass('hastabs');
			this.$modal.find('span.wbbm-title-text').html(opt.title);
			if( $.isFunction(opt.onClose) ) {
				this.$modal.data('onClose', opt.onClose);
			}
			if( opt.tabs && opt.tabs.length > 1 ) {
				$wbbm.addClass('hastabs');
				var $ul = $('<div class="wbbm-tablist"></div>').appendTo($cont).append('<ul/>').children('ul');
				$.each( opt.tabs, $.proxy( function(i, row) {
					if( i == 0 ) {
						row['on'] = 'on';
					}
					$ul.append( this.strf('<li class="{on}" onClick="$(this).parent().find(\'.on\').removeClass(\'on\');$(this).addClass(\'on\');$(this).parents(\'.wbbm-content\').find(\'.tab-cont\').hide();$(this).parents(\'.wbbm-content\').find(\'.tab' + i + '\').show()">{title}</li>', row) );
				}, this ) )
			}
			if(opt.width) {
				$wbbm.css('width',opt.width);
			}
			var $cnt = $('<div class="wbbm-cont"></div>').appendTo($cont);
			if(queryState) {
				$wbbm.find('#wbbm-remove').show();
			} else {
				$wbbm.find('#wbbm-remove').hide();
			}
			$.each( opt.tabs, $.proxy( function(i, r) {
				var $c = $('<div/>').addClass('tab-cont tab' + i).attr('tid', i).appendTo($cnt);
				if( i > 0 ) {
					$c.hide();
				} 
				if(r.html) {
					$c.html( this.strf(r.html, this.options) );
				} else {
					$.each(r.input, $.proxy( function(j, inp) {
						inp['value'] = queryState[ inp.param.toLowerCase() ];
						if( inp.param.toLowerCase() == 'seltext' && ( ! inp['value'] || inp['value'] == '') ) {
							inp['value'] = this.getSelectText(this.options.bbmode);
						}
						$c.append( this.strf('<div class="wbbm-inp-row"><label>{title}</label><input class="modal-text" type="text" name="{param}" id="uwbb-modal-seltext" value=""/></div>', inp) ).find('#uwbb-modal-seltext').val( inp['value'] );
					}, this ) );
				}
			}, this ) );
			this.lastRange=this.getRange();
			if( $.isFunction(opt.onLoad) ) {
				opt.onLoad.call(this, cmd, opt, queryState);
			}
			$wbbm.find('#wbbm-submit').click( $.proxy( function(event) {
				if( $.isFunction(opt.onSubmit) ) {
					var r = opt.onSubmit.call(this, cmd, opt, queryState);
					if( r === false ) {
						return;
					}
				}
				var params = {};
				var valid = true;
				this.$modal.find('.wbbm-inperr').remove();
				this.$modal.find('.wbbm-brdred').removeClass('wbbm-brdred');
				$.each( this.$modal.find('.tab-cont:visible input'), $.proxy( function(i, el) {
					var tid = $(el).parents('.tab-cont').attr('tid');
					var pname = $(el).attr('name').toLowerCase();
					var pval = $(el).val();
					var validation = opt.tabs[tid]['input'][i]['validation'];
					if( typeof validation != 'undefined' ) {
						if( ! pval.match( new RegExp(validation, 'i') ) ) {
							valid = false;
							$(el).after('<span class="wbbm-inperr">' + CURLANG.validation_err + '</span>').addClass('wbbm-brdred');
						}
					}
					params[pname] = pval;
				}, this ) );
				if(valid) {
					if( this.lastRange ) {
						this.selectRange(this.lastRange);
					}
					if(queryState) {
						this.wbbRemoveCallback(cmd, true);
					}
					this.wbbInsertCallback(cmd, params);
					this.closeModal();
					this.updateUI(event);
				}
			}, this ) );
			$wbbm.find('#wbbm-remove').click( $.proxy( function(event) {
				this.wbbRemoveCallback(cmd);
				this.closeModal();
				this.updateUI(event);
			}, this ) );
			$(document.body).css('overflow', 'hidden');
			if( $(document.body).height() > $(window).height() ) {
				$(document.body).css('padding-right', '20px');
			}
			this.$modal.show();
			$wbbm.css('margin-top', ( $(window).height() - $wbbm.outerHeight() ) / 3 + 'px');
			setTimeout( $.proxy( function() {
				this.$modal.find('input:visible')[0].focus();
			}, this ), 10 );
		},
		escModal: function(e) {
			if( e.which == 27 ) {
				this.closeModal();
			}
		},
		closeModal: function() {
			var onClose = this.$modal.data('onClose');
			if( onClose && $.isFunction(onClose) ) {
				onClose.call(this);
			}
			$(document.body).css('overflow', 'auto').css('padding-right', '0').unbind('keyup', this.escModal);
			this.$modal.find('#wbbm-submit, #wbbm-remove').unbind('click');
			this.$modal.hide();
			this.lastRange = false;
			return this;
		},
		getParams: function(src, s, offset) {
			var params = {};
			if(this.options.bbmode) {
				var stext = s.match(/\{[\s\S]+?\}/g);
				s = this.prepareRGX(s);
				var rgx = new RegExp(s, 'g');
				var val = this.txtArea.value;
				if( offset > 0 ) {
					val = val.substr( offset, val.length - offset );
				}
				var a = rgx.exec(val);
				if(a) {
					$.each( stext, function(i, n) {
						params[ n.replace(/\{|\}/g,'').replace(/"/g, "'").toLowerCase() ] = a[ i + 1 ];
					} );
				}
			} else {
				var rules = this.options.rules[s][0][1];
				$.each( rules, $.proxy( function(k, v) {
					var value = '';
					if( v.attr !== false) {
						value = $(src).attr(v.attr);
					} else if( v.sel !== false ) {
						value = $(src).find(v.sel).html();
					} else {
						value = $(src).html();
					}
					if(value) {
						if( v.rgx !== false ) {
							var m = value.match( new RegExp(v.rgx) );
							if( m && m.length == 2 ) {
								value = m[1];
							}
						}
						params[k] = value.replace(/"/g, "'");
					}
				}, this ) );
			}
			return params;
		},
		printObjectInIE: function(obj) {
			try{
				$.log( JSON.stringify(obj) );
			} catch(e) {}
		},
		checkFilter: function(node, filter) {
			$.log( "node: " + $(node).get(0).outerHTML + " filter: " + filter + " res: " + $(node).is( filter.toLowerCase() ) );
		},
		debug: function(msg) {
			if( typeof console != 'undefined' ) {
				console.log(msg);
			} else {
				$('#exlog').append('<p>' + msg + '</p>');  
			}
		}
	}
	$.log = function(msg) {
		if( typeof console != 'undefined' ) {
			console.log(msg);
		} else {
			$('#exlog').append('<p>' + msg + '</p>');  
		}
	}
	$.fn.wysibb = function(settings) {
		return this.each( function() {
			var data = $(this).data('wbb');
			if( ! data ) {
				new $.wysibb(this, settings);
			}
		} );
	}
	$.fn.getDoc = function() {
		return this.data('wbb').doc;
	}
	$.fn.getSelectText = function(fromTextArea) {
		return this.data('wbb').getSelectText(fromTextArea);
	}
	$.fn.bbcode = function(data) {
		if(data) {
			this.data('wbb').$txtArea.val(data);
			return this;
		} else {
			return this.data('wbb').getBBCode();
		}
	}
	$.fn.htmlcode = function(data) {
		if(data) {
			this.data('wbb').$body.html(data);
			return this;
		} else {
			return this.data('wbb').getHTML( this.data('wbb').$txtArea.val() );
		}
	}
	$.fn.getBBCode = function() {
		return this.data('wbb').getBBCode();
	}
	$.fn.getHTML = function() {
		var wbb = this.data('wbb');
		return wbb.getHTML( wbb.$txtArea.val() );
	}
	$.fn.getHTMLByCommand = function(command, params) {
		return this.data('wbb').getHTMLByCommand(command, params);
	}
	$.fn.getBBCodeByCommand = function(command,params) {
		return this.data('wbb').getBBCodeByCommand(command, params);
	}
	$.fn.insertAtCursor = function(data, forceBBMode) {
		this.data('wbb').insertAtCursor(data, forceBBMode);
	}
	$.fn.execCommand = function(command,value) {
		this.data('wbb').execCommand(command,value);
	}
	$.fn.insertImage = function(imgurl, thumburl) {
		var editor = this.data('wbb');
		var code = (thumburl) ? editor.getCodeByCommand('link', {
			url: imgurl,
			seltext: editor.getCodeByCommand( 'img', { src: thumburl } )
		} ) : editor.getCodeByCommand('img', { src: imgurl } );
		this.insertAtCursor(code);
		return editor;
	}
	$.fn.sync = function() {
		this.data('wbb').sync();
	}
	$.fn.destroy = function() {
		this.data('wbb').destroy();
	}
	$.fn.queryState = function(command) {
		this.data('wbb').queryState(command);
	}
	$.fn.uwbbClear = function() {
		this.data('wbb').clear();
	}
})(jQuery);
function no_jq_is_msie() {
	return window.navigator.userAgent.indexOf('MSIE ') == -1 ? false : true;
}
var ___code_transform = {};
var ___code_html = no_jq_is_msie() ? '<div data-code="1"><div style="padding-left: 5px;" contenteditable="false">'+UWBBLANG.code+':</div><div style="border:1px solid #DCDCDC; color:#828282; padding: 5px;">{SELTEXT}</div></div>' : '<fieldset style="border:1px solid #DCDCDC;" data-code="1"><legend contenteditable="false">'+UWBBLANG.code+'</legend><div style="color:#828282;" data-code="1">{SELTEXT}</div></div></fieldset>';
___code_transform[___code_html] = '[code]{SELTEXT}[/code]';
var ___hide_transform = {};
var ___hide_html = no_jq_is_msie() ? '<div data-hide="1"><div style="padding-left: 5px;" contenteditable="false">'+UWBBLANG.hideShort+':</div><div style="border:1px solid #DCDCDC; color:#828282; padding: 5px;">{SELTEXT}</div></div>' : '<fieldset style="border:1px solid #DCDCDC;" data-hide="1"><legend contenteditable="false">'+UWBBLANG.hideShort+'</legend><div style="color:#828282;" data-hide="1">{SELTEXT}</div></div></fieldset>';
___hide_transform[___hide_html] = '[hide]{SELTEXT}[/hide]';
var ___quote_transform = {};
var ___quote_html = [];
___quote_html[1] = no_jq_is_msie() ? '<div data-quote="1"><div style="padding-left: 5px;" contenteditable="false">'+UWBBLANG.quote+':</div><div style="border:1px solid #DCDCDC; color:#828282; padding: 5px;">{SELTEXT}</div></div>' : '<fieldset style="border:1px solid #DCDCDC;" data-quote="1"><legend contenteditable="false"><span contenteditable="false">'+UWBBLANG.quote+'</span></legend><div style="color:#828282;" data-quote="1">{SELTEXT}</div></div></fieldset>';
___quote_html[2] = no_jq_is_msie() ? '<div data-quote="1"><div style="padding-left: 5px;"><span contenteditable="false">'+UWBBLANG.quote+' </span><span style="font-weight:bold;">{AUTHOR}</span></div><div style="border:1px solid #DCDCDC; color:#828282; padding: 5px;">{SELTEXT}</div></div>' : '<fieldset style="border:1px solid #DCDCDC;" data-quote="1"><legend><span contenteditable="false">'+UWBBLANG.quote+' </span><span style="font-weight:bold;">{AUTHOR}</span></legend><div style="color:#828282;" data-quote="1">{SELTEXT}</div></div></fieldset>';
___quote_transform[___quote_html[1]] = '[quote]{SELTEXT}[/quote]';
___quote_transform[___quote_html[2]] = '[quote={AUTHOR}]{SELTEXT}[/quote]';
var ___spoiler_transform = {};
var ___spoiler_html = [];
___spoiler_html[1] = no_jq_is_msie() ? '<div data-spoiler="1"><div style="padding-left: 5px;" contenteditable="false">'+UWBBLANG.spoiler+':</div><div style="border:1px solid #DCDCDC; color:#828282; padding: 5px;">{SELTEXT}</div></div>' : '<fieldset style="border:1px solid #DCDCDC;" data-spoiler="1"><legend style="border: 1px solid #999; background-color: #EEE; padding: 2px 5px; border-radius: 2px;">' + UWBBLANG.spoiler + '</legend><div style="color:#828282;" data-spoiler="1">{SELTEXT}</div></fieldset>';
___spoiler_html[2] = no_jq_is_msie() ? '<div data-spoiler="1"><div style="padding-left: 5px;"><span contenteditable="false">'+UWBBLANG.spoiler+' </span><span style="font-weight:bold;">{TITLE}</span></div><div style="border:1px solid #DCDCDC; color:#828282; padding: 5px;">{SELTEXT}</div></div>' : '<fieldset style="border:1px solid #DCDCDC;" data-spoiler="1"><legend style="border: 1px solid #999; background-color: #EEE; padding: 2px 5px; border-radius: 2px;">{TITLE}</legend><div style="color:#828282;" data-spoiler="1">{SELTEXT}</div></fieldset>';
___spoiler_transform[___spoiler_html[1]] = '[spoiler]{SELTEXT}[/spoiler]';
___spoiler_transform[___spoiler_html[2]] = '[spoiler={TITLE}]{SELTEXT}[/spoiler]';
var ___justify = {};
if( $.browser.msie ) {
	___justify.center = {
		title: UWBBLANG.justifycenter,
		buttonHTML: '<span class="ve-tlb-textcenter"></span>',
		excmd: 'justifycenter',
		transform: {
			'<p align="center">{SELTEXT}</p>': '[c]{SELTEXT}[/c]'
		}
	};
	___justify.left = {
		title: UWBBLANG.justifycenter,
		buttonHTML: '<span class="ve-tlb-textleft"></span>',
		excmd: 'justifyleft',
		transform: {
			'<p align="left">{SELTEXT}</p>': '[l]{SELTEXT}[/l]'
		}
	};
	___justify.right = {
		title: UWBBLANG.justifycenter,
		buttonHTML: '<span class="ve-tlb-textright"></span>',
		excmd: 'justifyright',
		transform: {
			'<p align="right">{SELTEXT}</p>': '[r]{SELTEXT}[/r]'
		}
	};
	___justify.justify = {
		title: UWBBLANG.justify,
		buttonHTML: '<span class="ve-tlb-textjustify"></span>',
		transform: {
			'<p align="justify">{SELTEXT}</p>': '[j]{SELTEXT}[/j]'
		}
	};
} else {
	___justify.center = {
		title: UWBBLANG.justifycenter,
		buttonHTML: '<span class="ve-tlb-textcenter"></span>',
		transform: {
			'<div align="center">{SELTEXT}</div>': '[c]{SELTEXT}[/c]'
		}
	};
	___justify.left = {
		title: UWBBLANG.justifyleft,
		buttonHTML: '<span class="ve-tlb-textleft"></span>',
		transform: {
			'<div align="left">{SELTEXT}</div>': '[l]{SELTEXT}[/l]'
		}
	};
	___justify.right = {
		title: UWBBLANG.justifyright,
		buttonHTML: '<span class="ve-tlb-textright"></span>',
		transform: {
			'<div align="right">{SELTEXT}</div>': '[r]{SELTEXT}[/r]'
		}
	};
	___justify.justify = {
		title: UWBBLANG.justify,
		buttonHTML: '<span class="ve-tlb-textjustify"></span>',
		transform: {
			'<div align="justify">{SELTEXT}</div>': '[j]{SELTEXT}[/j]'
		}
	};
}
WBBPRESET = {
	onlyBBmode: false,
	themeName: 'default',
	bodyClass: '',
	tabInsert: true,
	imgupload: false,
	smileList: [],
	smilefind: false,
	hotkeys: false,
	showHotkeys: false,
	autoresize: false,
	debug: false,
	fastPaste: function(name,text){
		var obj = $('#wbbmodal');
		obj.find('input[name="'+name+'"]').val('http://');
		setTimeout(
			function(){ $('input[name="' + name + '"]').select(); },
			10
		);
		if(text)
			obj = obj.find('input[name="'+name+'"], input[name="SELTEXT"]');
		else obj = obj.find('input[name="'+name+'"]');
		obj.keydown(function(e){
			if(e.keyCode == 13) $('#wbbm-submit').click();
		});
	},
	allButtons: {
		justifyleft: ___justify.left,
		justifyright: ___justify.right,
		justifycenter: ___justify.center,
		justify: ___justify.justify,
		hr: {
			title: UWBBLANG.hr,
			buttonHTML: '<span class="ve-tlb-hr"></span>',
			transform: {
				'<hr>':'[hr]'
			}
		},
		br: {
			buttonText: UWBBLANG.br,
			transform: {
				'<br>':'[br]'
			}
		},
		hide: {
			title: UWBBLANG.hide,
			buttonHTML: '<span class="ve-tlb-hide"></span>',
			transform: ___hide_transform
		},
		spoiler: {
			title: UWBBLANG.spoiler,
			buttonHTML: '<span class="ve-tlb-spoiler"></span>',
			transform: ___spoiler_transform
		},
		code: {
			title: UWBBLANG.code,
			buttonHTML: '<span class="ve-tlb-code"></span>',
			transform: ___code_transform
		},
		quote: {
			title: UWBBLANG.quote,
			buttonHTML: '<span class="ve-tlb-quote"></span>',
			transform: ___quote_transform
		},
		fs_6: {
			title: "6pt",
			exvalue: null,
			buttonText: '6pt',
			transform: {
				'<span style="font-size: 6pt;">{SELTEXT}</span>':'[size=6]{SELTEXT}[/size]'
			}
		},
		fs_7: {
			title: "7pt",
			exvalue: null,
			buttonText: '7pt',
			transform: {
				'<span style="font-size: 7pt;">{SELTEXT}</span>':'[size=7]{SELTEXT}[/size]'
			}
		},
		fs_8: {
			title: "8pt",
			exvalue: null,
			buttonText: '8pt',
			transform: {
				'<span style="font-size: 8pt;">{SELTEXT}</span>':'[size=8]{SELTEXT}[/size]'
			}
		},
		fs_9: {
			title: "9pt",
			exvalue: null,
			buttonText: '9pt',
			transform: {
				'<span style="font-size: 9pt;">{SELTEXT}</span>':'[size=9]{SELTEXT}[/size]'
			}
		},
		fs_10: {
			title: "10pt",
			exvalue: null,
			buttonText: '10pt',
			transform: {
				'<span style="font-size: 10pt;">{SELTEXT}</span>':'[size=10]{SELTEXT}[/size]'
			}
		},
		fs_11: {
			title: "11pt",
			exvalue: null,
			buttonText: '11pt',
			transform: {
				'<span style="font-size: 11pt;">{SELTEXT}</span>':'[size=11]{SELTEXT}[/size]'
			}
		},
		fs_12: {
			title: "12pt",
			exvalue: null,
			buttonText: '12pt',
			transform: {
				'<span style="font-size: 12pt;">{SELTEXT}</span>':'[size=12]{SELTEXT}[/size]'
			}
		},
		fs_13: {
			title: "13pt",
			exvalue: null,
			buttonText: '13pt',
			transform: {
				'<span style="font-size: 13pt;">{SELTEXT}</span>':'[size=13]{SELTEXT}[/size]'
			}
		},
		fs_14: {
			title: "14pt",
			exvalue: null,
			buttonText: '14pt',
			transform: {
				'<span style="font-size: 14pt;">{SELTEXT}</span>':'[size=14]{SELTEXT}[/size]'
			}
		},
		fs_15: {
			title: "15pt",
			exvalue: null,
			buttonText: '15pt',
			transform: {
				'<span style="font-size: 15pt;">{SELTEXT}</span>':'[size=15]{SELTEXT}[/size]'
			}
		},
		fs_16: {
			title: "16pt",
			exvalue: null,
			buttonText: '16pt',
			transform: {
				'<span style="font-size: 16pt;">{SELTEXT}</span>':'[size=16]{SELTEXT}[/size]'
			}
		},
		fs_17: {
			title: "17pt",
			exvalue: null,
			buttonText: '17pt',
			transform: {
				'<span style="font-size: 17pt;">{SELTEXT}</span>':'[size=17]{SELTEXT}[/size]'
			}
		},
		fs_18: {
			title: "18pt",
			exvalue: null,
			buttonText: '18pt',
			transform: {
				'<span style="font-size: 18pt;">{SELTEXT}</span>':'[size=18]{SELTEXT}[/size]'
			}
		},
		fs_19: {
			title: "19pt",
			exvalue: null,
			buttonText: '19pt',
			transform: {
				'<span style="font-size: 19pt;">{SELTEXT}</span>':'[size=19]{SELTEXT}[/size]'
			}
		},
		fs_20: {
			title: "20pt",
			exvalue: null,
			buttonText: '20pt',
			transform: {
				'<span style="font-size: 20pt;">{SELTEXT}</span>':'[size=20]{SELTEXT}[/size]'
			}
		},
		fs_21: {
			title: "21pt",
			exvalue: null,
			buttonText: '21pt',
			transform: {
				'<span style="font-size: 21pt;">{SELTEXT}</span>':'[size=21]{SELTEXT}[/size]'
			}
		},
		fs_22: {
			title: "22pt",
			exvalue: null,
			buttonText: '22pt',
			transform: {
				'<span style="font-size: 22pt;">{SELTEXT}</span>':'[size=22]{SELTEXT}[/size]'
			}
		},
		fontsize: {
			type: 'select',
			title: UWBBLANG.fontsize,
			buttonHTML: '<span class="ve-tlb-size"></span>',
			options: "fs_6,fs_7,fs_8,fs_9,fs_10,fs_11,fs_12,fs_13,fs_14,fs_15,fs_16,fs_17,fs_18,fs_19,fs_20,fs_21,fs_22",
			excmd: 'fontSize'
		},
		ufontfamily: {
			type: 'select',
			title: UWBBLANG.fontfamily,
			excmd: 'fontName',
			valueBBname: "font",
			options: [
				{ title: "Arial",   exvalue: "Arial"   },
				{ title: "Times",   exvalue: "Times"   },
				{ title: "Courier", exvalue: "Courier" },
				{ title: "Impact",  exvalue: "Impact"  },
				{ title: "Geneva",  exvalue: "Geneva"  },
				{ title: "Optima",  exvalue: "Optima"  }
			],
			transform: {
				'<font face="{FONT}">{SELTEXT}</font>':'[font={FONT}]{SELTEXT}[/font]'
			}
		},
		video: {
			title: UWBBLANG.video,
			buttonHTML: '<span class="ve-tlb-video"></span>',
			modal: {
				title: UWBBLANG.video,
				width: "600px",
				tabs: [
					{
						title: UWBBLANG.video,
						input: [
							{
								param: "SRC",
								title: UWBBLANG.modal_video_text
							}
						]
					}
				],
				onLoad: function(){
					WBBPRESET.fastPaste('SRC',0);
				},
				onSubmit: function(cmd, opt, queryState) {
					var url = this.$modal.find('input[name="SRC"]').val();
					var a = url.match(/^(?:http:\/\/)?(?:w{3}\.)?youtube\.com\/watch\?v=([a-z0-9_-]+)/i) ||  url.match(/^(?:http:\/\/)?(?:w{3}\.)?youtu\.be\/([a-z0-9_-]+)/i);
					if(a && a.length==2) {
						this.insertAtCursor( this.getCodeByCommand( cmd, { src: a[1] } ) );
						this.closeModal();
						if( !this.options.bbmode ) {
							var ytIframes = this.$body.find('iframe');
							ytIframes.each(function() {
								var elem = $(this);
								if( elem.attr('_src') ) elem.attr('src', elem.attr('_src')).removeAttr('_src');
							});
						}
					} else {
						alert(UWBBLANG.video_url_error);
					}
					return false;
				},
				msiecallback: function(wbbCommand, wbbOptModal, wbbQueryState) {
					var videoURL = prompt( UWBBLANG.modal_video_text, '' );
					if( ! videoURL )
						return;
					var a = videoURL.match(/^(?:http:\/\/)?(?:w{3}\.)?youtube\.com\/watch\?v=([a-z0-9_-]+)/i) ||  videoURL.match(/^(?:http:\/\/)?(?:w{3}\.)?youtu\.be\/([a-z0-9_-]+)/i);
					if(a && a.length==2) {
						this.insertAtCursor( this.getCodeByCommand( wbbCommand, { src: a[1] } ) );
						if( !this.options.bbmode ) {
							var ytIframes = this.$body.find('iframe');
							ytIframes.each(function() {
								var elem = $(this);
								if( elem.attr('_src') ) elem.attr('src', elem.attr('_src')).removeAttr('_src');
							});
						}
					} else {
						alert(UWBBLANG.video_url_error);
					}
				}
			},
			transform: {
				'<iframe src="http://www.youtube.com/embed/{SRC}" width="640" height="480" frameborder="0"></iframe>':'[video]http://www.youtube.com/watch?v={SRC}[/video]'
			}
		},
		link: {
			title: UWBBLANG.link,
			buttonHTML: '<span class="ve-tlb-link"></span>',
			modal: {
				title: UWBBLANG.modal_link_title,
				width: "500px",
				tabs: [
					{
						input: [
							{ param: "URL", title: UWBBLANG.modal_link_url, validation: '^http(s)?://' },
							{ param: "SELTEXT", title: UWBBLANG.modal_link_text, validation: '^.'}
						]
					}
				],
				onLoad: function(){
					WBBPRESET.fastPaste('URL',1);
				},
				msiecallback: function(wbbCommand, wbbParams, wbbQueryState) {
					if(wbbQueryState) {
						this.wbbRemoveCallback(wbbCommand);
					} else if( ! this.getSelectText() ) {
						var _seltext = prompt(CURLANG.modal_link_text, '');
						if( ! _seltext ) {
							if( _seltext === false || _seltext === null ) {
								return;
							} else {
								alert(UWBBLANG.link_text_error);
								return;
							}
						}
						var _url = prompt(UWBBLANG.modal_link_url, 'http://');
						if( _url === false || _url === null ) {
							return;
						} else if(_url) {
							if( /^https?:\/\/.+$/i.test(_url) ) {
								this.wbbInsertCallback( wbbCommand, { seltext: _seltext, url: _url } );
							} else {
								alert(UWBBLANG.link_url_error);
								return;
							}
						} else {
							alert(UWBBLANG.link_url_error);
							return;
						}
					} else {
						var _url = prompt(CURLANG.modal_link_url, 'http://');
						if( _url === false || _url === null ) {
							return;
						} else if(_url) {
							if( /^https?:\/\/.+$/i.test(_url) ) {
								this.wbbInsertCallback( wbbCommand, { url: _url } );
							} else {
								alert(UWBBLANG.link_url_error);
								return;
							}
						} else {
							alert(UWBBLANG.link_url_error);
							return;
						}
					}
				}
			},
			transform: {
				'<a href="{URL}">{SELTEXT}</a>':"[url={URL}]{SELTEXT}[/url]"
			}
		},
		img: {
			title: UWBBLANG.img,
			buttonHTML: '<span class="ve-tlb-img"></span>',
			modal: {
				title: UWBBLANG.modal_img_title,
				width: "600px",
				tabs: true || location.pathname.indexOf('/forum/') != 0 ? [ // TMPIMG UPLOAD TODO
					{
						title: UWBBLANG.modal_img_tab1,
						input: [
							{param: "SRC", title: UWBBLANG.modal_imgsrc_text, validation: '^http(s)?://.*?\.(jpg|png|gif|jpeg)$'}
						]
					}
				] : [
					{
						title: UWBBLANG.modal_img_tab1,
						input: [
							{param: "SRC", title: UWBBLANG.modal_imgsrc_text, validation: '^http(s)?://.*?\.(jpg|png|gif|jpeg)$'}
						]
					},
					{
						title: UWBBLANG.modal_img_tab2,
						html: '<div id="tmpimg-modal"></div>'
					}
				],
				onLoad: function() {
					WBBPRESET.fastPaste('SRC', 0);
					this.tmpimg = {};
					this.tmpimg.modal = $('#tmpimg-modal');
					if( ! this.tmpimg.modal.length )
						return false;
					this.tmpimg.parent = $('#iplus').parent();
					this.tmpimg.modal.append( this.tmpimg.parent.contents() );
				},
				onClose: function() {
					if( ! this.tmpimg.modal.contents().length ) return false;
					this.tmpimg.parent.append( this.tmpimg.modal.contents() );
				},
				onSubmit: function() { if( ! $('.wbbm-cont input[name="SRC"]').val().length ) { this.closeModal(); return false; } },
				msiecallback: function(wbbCommand, wbbParams, wbbQueryState) {
					var _src = prompt(UWBBLANG.modal_imgsrc_text, 'http://');
					if( _src === false || _src === null)
						return;
					if(_src) {
						if( /^http(s)?:\/\/.*?\.(jpg|png|gif|jpeg)$/i.test(_src) ) {
							this.wbbInsertCallback(wbbCommand, { src: _src } );
						} else {
							alert(UWBBLANG.img_src_error);
							return;
						}
					} else {
						alert(UWBBLANG.img_src_error);
						return;
					}
				}
			},
			transform: {
				'<img src="{SRC}" />':"[img]{SRC}[/img]"
			},
			html: ''
		},
		bullist: {
			title: UWBBLANG.bullist,
			buttonHTML: '<span class="ve-tlb-list"></span>',
			excmd: 'insertUnorderedList',
			transform: {
				'<ul>{SELTEXT}</ul>':"[list]{SELTEXT}[/list]",
				'<li>{SELTEXT}</li>':"[*]{SELTEXT}\n"
			}
		}
	}
}
function _uCookie(cookieName, cookiePath) {
	this.cName = cookieName;
	this.cPath = cookiePath;
	this.name = function(cName) {
		if( ! ( typeof cName == 'string' || typeof cName == 'number' ) )
			return this.cName;
		this.cName = cName;
		return this;
	}
	this.path = function(cPath) {
		if( ! typeof cPath == 'string' )
			return this.cPath;
		this.cPath = cPath;
		return this;
	}
	this.set = function(cName, cValue, cDays, cPath) {
		if( ! ( cName = typeof cName == 'string' || typeof cName == 'number' ? cName : this.cName ) ) return false;
		var exDate = '';
		if(typeof cDays == 'number') {
			var exDate = new Date();
			exDate.setTime( exDate.getTime() + cDays*86400000 );
			exDate = '; expires=' + exDate.toGMTString();
		}
		document.cookie = cName + '=' + ( typeof cValue == 'number' || typeof cValue == 'string' ? escape(cValue) : '' ) + exDate + '; path=' + ( typeof cPath == 'string' ? cPath : (this.cPath || '/') );
		return true;
	}
	this.get = function(cName) {
		if( ! ( cName = typeof cName == 'string' || typeof cName == 'number' ? cName : this.cName ) ) return null;
		var allCookies = document.cookie.split(';');
		var indexOfEqSign = 0;
		for(cNum = 0; cNum < allCookies.length; cNum++) {
			if( (indexOfEqSign = allCookies[cNum].indexOf('=') ) == -1 ) continue;
			if( allCookies[cNum].substr(0, indexOfEqSign).replace(/^\s+|\s+$/g, '') == cName )
				return unescape( allCookies[cNum].substr(++indexOfEqSign) );
		}
		return null;
	}
	this.clear = function(cName, cPath) {
		return this.set(cName, null, null, cPath);
	}
	this.remove = function(cName, cPath) {
		return this.set(cName, null, -1, cPath);
	} 
}
function _uInfoStorage(_key, _namespace) {
	this.sKey = typeof _key == 'string' || typeof _key == 'number' ? _key : '_uInfoStorage';
	this.sNS = typeof _namespace == 'string' || typeof _namespace == 'number' ? _namespace : '';
	this.sType = typeof localStorage == 'undefined' || localStorage == null ? 0 : 1;
	this.uCookie = this.sType === 0 ? new _uCookie() : null;
	this.key = function(key) {
		if( ! ( typeof key == 'string' || typeof key == 'number' ) )
			return this.sKey;
		this.sKey = key;
		return this;
	}
	this.namespace = function(namespace) {
		if( ! ( typeof key == 'string' || typeof key == 'number' ) )
			return this.sNS;
		this.sNS = namespace;
		return this;
	}
	this.type = function(type) {
		if( typeof type != 'number' || ! (type === 1 || type === 0) )
			return this.sType;
		this.sType = type;
		if(this.sType === 0 && ! this.uCookie) {
			this.uCookie = new _uCookie();
		}
		return this;
	}
	this.forceCookies = function() {
		return this.type(0);
	}
	this.init = function(key, namespace, forceCookies) {
		this.key(key);
		this.namespace(namespace);
		this.sType = forceCookies ? 0 : localStorage ? 1 : 0;
		this.type( this.sType );
		return this;
	}
	this.save = function(info, key, namespace) {
		if( ! ( key = typeof key == 'string' || typeof key == 'number' ? key : this.sKey ) )
			return false;
		if(namespace != 2) namespace = namespace = typeof namespace == 'string' || typeof namespace == 'number' ? namespace : ( this.sNS || '' );
		if(this.sType === 1)
			return localStorage.setItem( namespace + '' + key, info ) || true;
		return this.uCookie.set( namespace + '' + key, info);
	}
	this.get = function(key, namespace) {
		if( ! ( key = typeof key == 'string' || typeof key == 'number' ? key : this.sKey ) )
			return null;
		if(namespace != 2) namespace = namespace = typeof namespace == 'string' || typeof namespace == 'number' ? namespace : ( this.sNS || '' );
		else namespace = '';
		if(this.sType === 1)
			return localStorage.getItem( namespace + '' + key );
		return this.uCookie.get( namespace + '' + key );
	}
	this.clear = function(key, namespace) {
		if( ! ( key = typeof key == 'string' || typeof key == 'number' ? key : this.sKey ) )
			return false;
		namespace = namespace = typeof namespace == 'string' || typeof namespace == 'number' ? namespace : ( this.sNS || '' );
		if(this.sType === 1)
			return localStorage.setItem( namespace + '' + key, '' ) || true;
		return this.uCookie.clear( namespace + '' + key );
	}
	this.remove = function(key, namespace) {
		if( ! ( key = typeof key == 'string' || typeof key == 'number' ? key : this.sKey ) )
			return false;
		namespace = namespace = typeof namespace == 'string' || typeof namespace == 'number' ? namespace : ( this.sNS || '' );
		if(this.sType === 1)
			return localStorage.removeItem( namespace + '' + key ) || true;
		return this.uCookie.remove( namespace + '' + key );
	}
}
$(
	function() {
		$.wysibb.prototype.infoStorage = null;
		$.wysibb.prototype.asintID = null;
		$.wysibb.prototype.dontsave = false;
		$.wysibb.prototype.wbbExecCommand = function(wbbCommand, wbbValue, wbbQueryState) {
			var wbbOpt = this.options.allButtons[wbbCommand];
			if(wbbOpt.modal) {
				if( $.isFunction(wbbOpt.modal) ) {
					wbbOpt.modal.call(this, wbbCommand, wbbOpt.modal, wbbQueryState);
				} else {
					if( $.browser.msie && ( typeof wbbOpt.modal.msiecallback == 'function' ) ) {
						wbbOpt.modal.msiecallback.call(this, wbbCommand, wbbOpt.modal, wbbQueryState);
					} else {
						this.showModal.call(this, wbbCommand, wbbOpt.modal, wbbQueryState);
					}
				}
			} else {
				if( wbbCommand.indexOf('fs_') == 0 ) {
					var curSize = 'fs_' + parseInt( this.$toolbar.find('.wbb-fontsize .val').text() );
					if( curSize.toLowerCase() != 'fs_nan' )
						this.wbbRemoveCallback(curSize);
					this.wbbInsertCallback(wbbCommand);
				} else if( wbbQueryState && wbbOpt.subInsert != true ) {
					this.wbbRemoveCallback(wbbCommand);
				} else {
					this.wbbInsertCallback(wbbCommand, wbbValue)
				}
			}
		}
		$.wysibb.prototype.sync = function() {
			if(this.options.bbmode && this.doc) {
				var wbbHTML = this.getHTML( this.txtArea.value, true );
				this.$body.html( wbbHTML );
			} else {
				if($.browser.opera) {
					var msg = this.$body.html();
					msg = msg.split('</p>');
					for(i=0;i<msg.length;i++) {
						if(msg[i].substr(msg[i].length-4,msg[i].length) == '<br>') msg[i] = msg[i].substr(0,msg[i].length-4);
						msg[i] = msg[i].replace(/<p>(.*)/g,"$1<br>");
					}
					msg = msg.join('');
					this.$body.html(msg);
				}
				var wbbBB = this.getBBCode();
				wbbBB = wbbBB.replace(/\] \[/gi, ']&nbsp;[');
				this.$txtArea.val(wbbBB);
			}
		}
		var wbbProtoToBB = $.wysibb.prototype.toBB;
		$.wysibb.prototype.toBB = function(wbbData) {
			var wbbToBB = wbbProtoToBB.call(this, wbbData);
			if( typeof wbbData != 'string' )
				return wbbToBB;
			if( /^\s*(?:\[br\]|\n)+\s*$/i.test(wbbToBB) ) return '';
			wbbToBB = wbbToBB.replace(/\[([a-z]+)=[\s\n]*\]/i, '[$1]').replace( /(\[[a-z]+(?:=[^\]]+)?\])(\s*)/gi, "$2$1" ).replace( /(\s*)(\[\/[a-z]+\])/gi, "$2$1" ).replace(/(\[*\][^\n]*?)\[\/list\]/g, "$1\n[/list]");
			return wbbToBB;
		}
		var wbbProtoBuildSmilebox = $.wysibb.prototype.buildSmilebox;
		$.wysibb.prototype.buildSmilebox = function(wbbContainer, wbbButtonName, wbbOpt) {
			wbbProtoBuildSmilebox.call(this, wbbContainer, wbbButtonName, wbbOpt);
			var wbbSmilesBlock = $( '.wbb-' + wbbButtonName + ' .wbb-list' );
			var wbbThis = this;
			$('<span id="_uwbb-add-smiles-button">' + UWBBLANG.smiles_additional + '</span>').click(
				function() {
					$(this).unbind('click');
					$(this).parents('.wbb-' + wbbButtonName).click();
					$(this).after('<div id="_uwbb-wait-add-smiles" title="' + UWBBLANG.loading + '"></div>').remove();
					$.get(
						'/index/35-1-0',
						function(response) {
							wbbThis.options.smileList = Array();
							wbbThis.options.srules = {};
							var uSmilesObj = $( 'tr', $('cmd[p="content"]', response).text() );
							for(i = 0; i < uSmilesObj.length; i++) {
								var uSmileIMG = uSmilesObj.eq(i).find('td:last img');
								var uSmileIMG_wbb = '<img src="' + uSmileIMG.attr('src') + '" class="sm" data-type="smilelist">';
								var uSmileBBCode;
								if( /\.gif$/i.test( uSmileIMG.attr('title') ) ) {
									uSmileBBCode = '[img]' + uSmileIMG.attr('src') + '[/img]';
								} else {
									uSmileBBCode = uSmilesObj.eq(i).find('td:first').html().replace(/</g, '&lt;');
								}
								wbbThis.options.smileList.push(
									{
										bbcode: uSmileBBCode,
										img: uSmileIMG_wbb,
										title: uSmileIMG.attr('title')
									}
								);
								wbbThis.options.srules[ wbbThis.filterByNode( $( wbbThis.strf(uSmileIMG_wbb, wbbThis.options) ) ) ] = [uSmileBBCode, uSmileIMG_wbb];
							}
							if( ! wbbThis.options.smileList.length ) {
								wbbSmilesBlock.find('#_uwbb-wait-add-smiles').remove();
								return;
							}
							wbbSmilesBlock.empty();
							$.each(
								wbbThis.options.smileList,
								$.proxy(
									function(i, sm) {
										$('<span>').addClass("smile").prependTo(wbbSmilesBlock).append( $( wbbThis.strf(sm.img, wbbThis.options) ).attr("title", sm.title) );
									},
									wbbThis
								)
							);
							wbbSmilesBlock.find('.smile').click(
								$.proxy(
									function(e) {
										this.insertAtCursor((this.options.bbmode) ? ' ' + this.toBB(e.currentTarget) + ' ' : ' ' + $(e.currentTarget).html() + '&nbsp;' );
									},
									wbbThis
								)
							);
						}
					);
				}
			).appendTo(wbbSmilesBlock);
		}
		$.wysibb.prototype.restore = function() {
			var storedHTML = this.infoStorage.get();
			var editorHTML = this.getHTML( this.$txtArea.val() );
			if( storedHTML && ( ! editorHTML || /^\s*<\/?\s*br>\s*$/i.test(editorHTML) ) )
				this.$body.html(storedHTML);
			if( this.infoStorage.get('2-uwbb-bbmode',2) == 1 ) {
				$('.modeSwitch').children('.wysibb-toolbar-btn').addClass("on");
				this.modeSwitch(true);
			}
			return this;
		}
		$.wysibb.prototype.save = function() {
			if( this.dontsave ) {
				this.infoStorage.clear();
			} else {
				this.infoStorage.save( this.getHTML( this.$txtArea.val(), false, true ) );
				this.infoStorage.save( this.options.bbmode ? 1 : 0 , '-uwbb-bbmode',2);
			}
			return this;
		}
		$.wysibb.prototype.autosave = function(interval) {
			if(this.asintID) {
				clearInterval(this.asintID);
				this.dontsave = ! this.dontsave;
				this.infoStorage.remove();
			} else {
				var wbbThis = this;
				$(window).unload(
					function() {
						wbbThis.save();
					}
				);
				this.asintID = setInterval(
					function() {
						wbbThis.save();
					},
					typeof interval == 'number' ? interval : 3000
				);
			}
			return this;
		}
		var wbbProtoInit = $.wysibb.prototype.init;
		$.wysibb.prototype.init = function() {
			if( typeof window.uCoz != 'object' || window.uCoz == null ) {
				window.uCoz = {};
			}
			if( typeof window.uCoz.uwbb != 'object' || window.uCoz.uwbb == null ) {
				window.uCoz.uwbb = {};
			}
			if( typeof window.uCoz.uwbb.instances != 'object' || window.uCoz.uwbb.instances == null ) {
				window.uCoz.uwbb.instances = [];
			}
			window.uCoz.uwbb.instances.push(this);
			if( typeof window.uCoz.uwbb.reAssigned != 'object' || window.uCoz.uwbb.reAssigned == null ) {
				window.uCoz.uwbb.reAssigned = {};
			}
			this.infoStorage = new _uInfoStorage(
				'-uwbb-' + ( this.$txtArea.attr('id') || this.$txtArea.attr('name') ) + '-' + ( this.$txtArea.parents('form').find('input[name="pid"]').val() || this.$txtArea.parents('form').find('input[name="s"]').val() || 'main' ),
				location.pathname.replace(/[^a-z0-9]/gi, '-')
			);
			var wbbThis = this;
			$('.postQuote').click(
				function() {
					var _author = $(this).attr('href').substr(19);
					_author = _author.substr( 0, _author.indexOf('\'') );
					if( ! $('#message:visible').length ) {
						if( window.selection && window.selection != '' ) {
						$('.wysibb-text-iframe').contents().find('.wysibb-body').focus();
							wbbThis.execCommand(
								'quote',
								{ author: _author, seltext: selection }
							);
						} else _uWnd.alert(UWBBLANG['select_text_for_quote'],'',{w:230,h:80,tm:3000});
					} else Insert(_author, selection);
					return false;
				}
			);
			$('.postUser').each( function() {
				var elem = $(this);
				var _nick = elem.attr('onclick');
				if( ! _nick ) {
					return;
				}
				_nick = _nick.substr(13);
				_nick = _nick.substr( 0, _nick.indexOf('\'') - 5 );
				elem.attr('post-author', _nick);
				elem.removeAttr('onclick');
			} );
			$('.postUser').click(
				function(e) {
					wbbThis.$body.focus();
					var _nick = $(this).attr('post-author');
					if( wbbThis.options.bbmode ) {
						wbbThis.insertAtCursor( '[b]' + _nick + '[/b],' + "\u00A0" );
					} else {
						wbbThis.insertAtCursor( '<b>' + _nick + '</b>,' + "\u00A0" );
					}
					return false;
				}
			);
			$('input.codeButtons').parent('span[id^="bc"]').parent('div').remove();
			var isComments = this.$txtArea.parents('form[onsubmit^="addcom"]');
			if( ! isComments.length ) {
				isComments = this.$txtArea.parents('#acform');
			}
			var isCommentsAnswer = this.$txtArea.parents('form[onsubmit^="sendFrm235"]');
			var isPM = this.$txtArea.parents('form[onsubmit^="safe"]');
			var isGb = this.$txtArea.parents('form[onsubmit^="gbsafe"]');
			var isForum = this.$txtArea.parents('form[onsubmit^="try"]');
			var uSmilesBlock = $('.sml1').closest('table');
			var isSmiles = (uSmilesBlock.length > 0) && ! ( this.options.bitmask & 1 );
			if( window.uCoz && window.uCoz.uwbb && window.uCoz.uwbb.smiles ) {
				this.options.smileList = window.uCoz.uwbb.smiles;
			}
			var wbbThis = this;
			if(uSmilesBlock.length && ! uSmilesBlock.data('uwbb-integrated') ) {
				var uSmilesObj = uSmilesBlock.find('.sml1');
				this.options.smileList = Array();
				if( ! window.uCoz ) window.uCoz = {};
				if( ! window.uCoz.uwbb ) window.uCoz.uwbb = {};
				if( ! window.uCoz.uwbb.smiles ) window.uCoz.uwbb.smiles = [];
				for( i = uSmilesObj.length-1; i > -1 ; i-- ) {
					var uSmileBB = uSmilesObj.eq(i).find('a').attr('onclick');
					if( ! uSmileBB ) {
						continue;
					}
					var uSmileIMG = uSmilesObj.eq(i).find('img');
					var currentSmile = {
						bbcode: uSmileBB.substr( 10, uSmileBB.substr(10).indexOf('\',') ).replace(/\\/g, '').replace(/</g, '&lt;'),
						img: '<img src="' + uSmileIMG.attr('src') + '" class="sm" data-type="smilelist">',
						title: uSmileIMG.attr('title')
					};
					this.options.smileList.push(currentSmile);
					window.uCoz.uwbb.smiles.push(currentSmile);
					uSmilesObj.eq(i).find('a').removeAttr('onclick').click( { 'smile': currentSmile }, function(e) {
						if( wbbThis.options.bbmode ) {
							wbbThis.insertAtCursor(' ' + e.data.smile.bbcode + ' ');
						} else {
							wbbThis.insertAtCursor(' ' + e.data.smile.img + '&nbsp;');
						}
					} );
				}
				var processSmiles = function(smilesObject) {
					wbbThis.options.smileList = Array();
					wbbThis.options.srules = {};
					var uSmilesObj = $( 'tr', smilesObject );
					for(i = 0; i < uSmilesObj.length; i++) {
						var uSmileIMG = uSmilesObj.eq(i).find('td:last img');
						var uSmileIMG_wbb = '<img src="' + uSmileIMG.attr('src') + '" class="sm" data-type="smilelist">';
						var uSmileBBCode;
						if( /\.gif$/i.test( uSmileIMG.attr('title') ) ) {
							uSmileBBCode = '[img]' + uSmileIMG.attr('src') + '[/img]';
						} else {
							uSmileBBCode = uSmilesObj.eq(i).find('td:first').html().replace(/</g, '&lt;');
						}
						var currentSmile = {
							bbcode: uSmileBBCode,
							img: uSmileIMG_wbb,
							title: uSmileIMG.attr('title')
						};
						wbbThis.options.smileList.push(currentSmile);
						wbbThis.options.srules[ wbbThis.filterByNode( $( wbbThis.strf(uSmileIMG_wbb, wbbThis.options) ) ) ] = [uSmileBBCode, uSmileIMG_wbb];
						uSmilesObj.eq(i).removeAttr('onclick').click( { 'smile': currentSmile }, function(e) {
							if( wbbThis.options.bbmode ) {
								wbbThis.insertAtCursor(' ' + e.data.smile.bbcode + ' ');
							} else {
								wbbThis.insertAtCursor(' ' + e.data.smile.img + '&nbsp;');
							}
						} );
					}
					if( ! wbbThis.options.smileList.length ) {
						return;
					}
				}
				var uAllSmiles = $('#allSmiles a');
				uAllSmiles.removeAttr('onclick').click(
					function() {
						var allSmilesTable = $('#ucoz-additional-smiles-table');
						var smilesWnd = function() {
							new _uWnd('Sml',' ',-250,-350,{autosize:0,closeonesc:1,resize:0, oncontent:function() {
								allSmilesTable.show();
							}, onbeforeclose: function() {
								allSmilesTable.hide().insertAfter(uAllSmiles);
							} }, allSmilesTable );
						}
						if( ! allSmilesTable.get(0) ) {
							$.get(
								'/index/35-1-0',
								function(response) {
									response = $( $('cmd[p="content"]', response).text() );
									allSmilesTable = $('<div/>').attr('id', 'ucoz-additional-smiles-table').hide().append(response).insertAfter(uAllSmiles);
									processSmiles.call(this, response);
									smilesWnd.call();
								}
							);
						} else {
							smilesWnd.call();
						}
					}
				);
				uSmilesBlock.data('uwbb-integrated', true);
			}
			if(isComments.length > 0) {
				if(isSmiles) {
					var parentTd = uSmilesBlock.parent('td');
					uSmilesBlock.remove();
					parentTd.remove();
				}
			} else if(isCommentsAnswer.length > 0) {
				if(isSmiles) uSmilesBlock.parent('td').remove();
			} else if(isPM.length > 0) {
				if(isSmiles) uSmilesBlock.parent('td').remove();
			} else if(isGb.length > 0) {
				if(isSmiles) uSmilesBlock.parent('td').remove();
			} else {
				if(isSmiles) uSmilesBlock.remove();
			}
			wbbProtoInit.call(this);
			$('.smile').parent('.wbb-list').css('text-align','center');
			if($.browser.chrome || $.browser.webkit) {
				this.restore();
			} else {
				this.$iFrame.bind(
					'load',
					$.proxy(
						function() {
							this.restore();
						},
						this
					)
				);
			}
			this.infoStorage.namespace( this.$txtArea.attr('id') );
			this.autosave();
				$(document).bind(
					'ajaxComplete',
					function(event, xhr, ajaxOpt) {
						if( $(ajaxOpt._formobj).is( wbbThis.$txtArea.parents("form") ) ) {
							if( xhr.responseText.indexOf('<span class="myWinError">*</span>') !== -1 ) {
								wbbThis.$body.html('');
								wbbThis.sync();
							}
						}
					}
				);
			if( window.spages ) {
				eval( 'window.spagesAJAX = ' + window.spages.toString().replace( /if\s*\(1\)/, 'if(0)' ) );
			} else {
				window.spages = function() { return false };
				window.spagesAJAX = function() { return false };
			}
			if( /^ipd/i.test( this.$txtArea.parents('form').attr('id') ) ) {
				this.$txtArea.parents('form').attr('onsubmit', "$('textarea[name=message], textarea[name=answer]').sync();" + ( this.$txtArea.parents('form').attr('onsubmit') || '' ) );
				$(document).bind(
					'ajaxComplete',
					function(event, xhr, ajaxOpt) {
						if( $(ajaxOpt._formobj).is( wbbThis.$txtArea.parents("form") ) && ! window.spagesAjaxIsWorking ) {
							window.spagesAjaxIsWorking = true;
							setTimeout( 
								function() {
									spagesAJAX( $('.pagesBlock1 .swchItemA').text() || '1' );
									window.spagesAjaxIsWorking = false;
								},
							500);
						}
					}
				);
			}
		}
		$.wysibb.prototype.prepareRGX = function(r) {
			return r.replace(/(\[|\]|\)|\(|\.|\*|\?|\:|\||\\|\^)/g,"\\$1").replace(/\{.*?\}/g,"([\\s\\S]*?)");
		}
		var wbbProtoDebug = $.wysibb.prototype.debug;
		$.wysibb.prototype.debug = function(message) {
			if(WBBPRESET.debug)
				wbbProtoDebug.call(this, message);
		}
		$.wysibb.prototype.clear = function() {
			this.$body.empty();
			this.$txtArea.val('');
			this.infoStorage.clear();
		}
	}
);
