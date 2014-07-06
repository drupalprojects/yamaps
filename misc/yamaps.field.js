/* 
 * @file
 * Script helps to work with multiple fields.
 */

(function($) {
  Drupal.behaviors.yamapsField = {
    attach: function (context, settings) {
      $('div.form-wrapper').delegate('.remove_yamap_button', 'click', function() {
        // Get parent table row.
        var row = $(this).closest('td').parent('tr');

        // Hide and empty value.
        $(row).hide().find('input').val('');

        // Fix table row classes.
        var table_id = $(row).parent('tbody').parent('table').attr('id');
        $('#' + table_id + ' tr.draggable:visible').each(function (index, element) {
          $(element).removeClass('odd').removeClass('even');
          if ((index % 2) == 0) {
            $(element).addClass('odd');
          }
          else {
            $(element).addClass('even');
          }
        });
      });
	  }
  }
})(jQuery);
