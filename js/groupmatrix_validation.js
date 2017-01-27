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
    var partner_badge_name = 15;

    function validateForm() {
      // SELECT select[name='submitted[group_registration_table][*][<partner>]']
      $("select[name^='submitted[group_registration_table][']")
        .filter(function() {return this.name.endsWith("][" + partner + "]");})
        .each(function() {
          var row = parseInt(this.name.substr(36));
          console.log(row);
          var partner = $("select[name='submitted[group_registration_table][" + row + "][" + partner + "]']");
          console.log(partner.val());

          if (parseInt(partner.val())) {
            // partner is ACTIVE: show fields
            $("select[name='submitted[group_registration_table][" + row + "][" + partner_prefix + "]']").parent().parent().show();
            $("input[name='submitted[group_registration_table][" + row + "][" + partner_first_name + "]']").parent().parent().show();
            $("input[name='submitted[group_registration_table][" + row + "][" + partner_last_name + "]']").parent().parent().show();
            $("input[name='submitted[group_registration_table][" + row + "][" + partner_badge_name + "]']").parent().parent().show();

          } else {
            // partner is INACTIVE: hide fields
            $("select[name='submitted[group_registration_table][" + row + "][" + partner_prefix + "]']").parent().parent().hide();
            $("input[name='submitted[group_registration_table][" + row + "][" + partner_first_name + "]']").parent().parent().hide();
            $("input[name='submitted[group_registration_table][" + row + "][" + partner_last_name + "]']").parent().parent().hide();
            $("input[name='submitted[group_registration_table][" + row + "][" + partner_badge_name + "]']").parent().parent().hide();
          }
        });
    }

    // // CATCH select[name='submitted[group_registration_table][*][<partner>]']
    //   $("select[name^='submitted[group_registration_table][']")
    //     .filter(function() {return this.name.endsWith("][" + partner + "]");})
    //     .change(validateForm);

    // catch ALL change events on table for processing
    $("body").change(validateForm);
  });
})(jQuery);


