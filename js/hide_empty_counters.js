/**
 * @file hide_empty_counters.js
 */

(function ($) {

  $("#edit-submitted-registration-member-int-count[value=0]").parent().hide();
  $("#edit-submitted-registration-local-count[value=0]").parent().hide();
  $("#edit-submitted-registration-participant-count[value=0]").parent().hide();
  $("#edit-submitted-registration-youth-count[value=0]").parent().hide();
  $("#edit-submitted-registration-partner-count[value=0]").parent().hide();
  
  $("#edit-submitted-registration-member-int-subtotal[value=0]").parent().hide();
  $("#edit-submitted-registration-local-subtotal[value=0]").parent().hide();
  $("#edit-submitted-registration-participant-subtotal[value=0]").parent().hide();
  $("#edit-submitted-registration-youth-subtotal[value=0]").parent().hide();
  $("#edit-submitted-registration-partner-subtotal[value=0]").parent().hide();

})(jQuery);
