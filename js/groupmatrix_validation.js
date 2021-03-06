/*-------------------------------------------------------+
| ICA Event Registration Module                          |
| Copyright (C) 2016 SYSTOPIA                            |
+--------------------------------------------------------+
| This program is released as free software under the    |
| Affero GPL license. You can redistribute it and/or     |
| modify it under the terms of this license which you    |
| can read by viewing the included agpl.txt or online    |
| at www.gnu.org/licenses/agpl.html. Removal of this     |
| copyright header is strictly prohibited without        |
| written permission from the original author(s).        |
+--------------------------------------------------------*/


// Groupmatrix validation
(function($) {
  $(document).ready(function() {
    var partner = 11;
    var partner_prefix = 12;
    var partner_first_name = 13;
    var partner_last_name = 14;
    var partner_badge = 15;

    function validateForm() {
      // SELECT select[name='submitted[group_registration_table][*][<partner>]']
      $("select[name^='submitted[group_registration_table][']")
        .filter(function() {return this.name.endsWith("][" + partner + "]");})
        .each(function() {
          var row = parseInt(this.name.substr(36));
          // console.log("ROW " + row);

          if (parseInt($(this).val())) {
            // partner is ACTIVE: show fields
            $("select[name='submitted[group_registration_table][" + row + "][" + partner_prefix + "]']").parent().parent().show(200);
            $("input[name='submitted[group_registration_table][" + row + "][" + partner_first_name + "]']").parent().parent().show(200);
            $("input[name='submitted[group_registration_table][" + row + "][" + partner_last_name + "]']").parent().parent().show(200);
            $("input[name='submitted[group_registration_table][" + row + "][" + partner_badge + "]']").parent().parent().show(200);

            // validate fields
            var prefix_field = $("select[name='submitted[group_registration_table][" + row + "][" + partner_prefix + "]']");
            if (parseInt(prefix_field.val())) {
              prefix_field.removeClass("ica-warning");
            } else {
              prefix_field.addClass("ica-warning");
            }

            var first_name_field = $("input[name='submitted[group_registration_table][" + row + "][" + partner_first_name + "]']");
            if (first_name_field.val().replace(/ +/g, "").length) {
              first_name_field.removeClass("ica-warning");
            } else {
              first_name_field.addClass("ica-warning");
            }
            
            var last_name_field = $("input[name='submitted[group_registration_table][" + row + "][" + partner_last_name + "]']");
            if (last_name_field.val().replace(/ +/g, "").length) {
              last_name_field.removeClass("ica-warning");
            } else {
              last_name_field.addClass("ica-warning");
            }

            var badge_name_field = $("input[name='submitted[group_registration_table][" + row + "][" + partner_badge + "]']");
            if (badge_name_field.val().replace(/ +/g, "").length) {
              badge_name_field.removeClass("ica-warning");
            } else {
              badge_name_field.addClass("ica-warning");
            }


          } else {
            // partner is INACTIVE: hide fields
            $("select[name='submitted[group_registration_table][" + row + "][" + partner_prefix + "]']").parent().parent().hide(200);
            $("input[name='submitted[group_registration_table][" + row + "][" + partner_first_name + "]']").parent().parent().hide(200);
            $("input[name='submitted[group_registration_table][" + row + "][" + partner_last_name + "]']").parent().parent().hide(200);
            $("input[name='submitted[group_registration_table][" + row + "][" + partner_badge + "]']").parent().parent().hide(200);
          }
        });
    }

    // catch ALL change events on table for processing
    $("body").change(validateForm);
    $(document).ready(validateForm);
    validateForm();
  });
})(jQuery);


