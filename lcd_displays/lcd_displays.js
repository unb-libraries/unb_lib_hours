
if (Drupal.jsEnabled) {
	$(document).ready(function() {
		$('.accordion div.window').filter(':not(.active)').hide() ;
		$('.accordion li a.ac_header').click(function() {
			var panel = $(this).next() ;
			if (!panel.is(':visible')) {
				panel.html('<img src="/core/img/loader.gif"/>');
			} 
			$(this).next().slideToggle('slow', function(){
				if ($(this).is(':visible')) {
					var elt = $(this) ;
					var url = Drupal.settings.lcd_displays.ahah_url + '/' + $(this).parent().attr('id') ; 
					$.get(url, {}, function(data){
						if (data == '') {
							elt.html("No items available") ;
						}
						else {
							elt.html(data) ;
						}
					}, 'html') ;
				} 
			}) ;
			
			return false ;
		}) ;
	}) ;
}