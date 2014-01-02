
var DateRange = {} ;

if (Drupal.jsEnabled) {
	$(document).ready(function() {
		$('.daterange-view-link').click(function() {
			DateRange.getView($(this));
			return false ;
		}) ;
	}) ;
	
	DateRange.getView = function(element) {
		var url = element.attr('href') ;
		if (element.attr('hasView')) {
			element.text('View') ;
			element.attr('hasView', '') ;
			element.parents().filter('tr:first').next('tr').slideUp('slow').remove() ;
		}
		else {
			$.post(Drupal.settings.daterange.view_url, {url: url}, function(data) {
				if (data == '') return ;
				element.parents().filter('tr:first').after(data).slideDown('slow') ;
				element.text('Hide') ;
				element.attr('hasView', true) ;
			}, 'html') ;
		}
	}
}