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


// General prefill functions
(function($) {
  $(document).ready(function(){

    // define helper functions
    function setRegistrantBatchName() {
      setBatchName("registrant");
    }

    function setPartnerBatchName() {
      setBatchName("partner");
    }

    function setBatchName(prefix) {
      var formal_title = $("input[name='submitted[" + prefix + "_formal_title]']").val();
      var first_name   = $("input[name='submitted[" + prefix + "_first_name]']").val();
      var last_name    = $("input[name='submitted[" + prefix + "_last_name]']").val();
      var badge_name   = formal_title + " " + first_name + " " + last_name;

      $("input[name='submitted[" + prefix + "_badge]']").val(badge_name.trim());
    }

    function setOrganisationBatchName() {
      var organisation_name = $("input[name='submitted[organisation_organization_name]']").val();
      var badge_name = organisation_name.trim().substring(0, 32);
      $("input[name='submitted[registrant_organisation_badge]']").val(badge_name);
    }


    // prefill email
    var registrant_email = $("input[name='submitted[registrant_email]']");
    if (registrant_email.val() == '') {
      registrant_email.val(Drupal.settings.ica_event_cmrf_connector.variables.email);
    }

    // add triggers
    $("input[name='submitted[registrant_formal_title]']").change(setRegistrantBatchName);
    $("input[name='submitted[registrant_first_name]']").change(setRegistrantBatchName);
    $("input[name='submitted[registrant_last_name]']").change(setRegistrantBatchName);
    $("input[name='submitted[partner_formal_title]']").change(setPartnerBatchName);
    $("input[name='submitted[partner_first_name]']").change(setPartnerBatchName);
    $("input[name='submitted[partner_last_name]']").change(setPartnerBatchName);
    $("input[name='submitted[organisation_organization_name]']").change(setOrganisationBatchName);
  });
})(jQuery);





// Matrix prefill functions
(function($) {
  $(document).ready(function(){
    // matrix mapping
    var matrix_triggers = {
      '2': 'registrant_',
      '4': 'registrant_',
      '3': 'registrant_',
      '13': 'partner_',
      '14': 'partner_',
    };

    var matrix_index = {
      'registrant_formal_title': 2,
      'registrant_first_name': 3,
      'registrant_last_name': 4,
      'registrant_badge': 5,
      'registrant_organisation_badge': 9,
      'partner_first_name': 13,
      'partner_last_name': 14,
      'partner_badge': 15,
    };


    function getFieldValue(field_key, row) {
      var index = matrix_index[field_key];
      if (index) {
        var selector = "input[name='submitted[group_registration_table][" + row + "][" + index + "]']";
        // console.log("getting " + selector);
        return $(selector).val();
      } else {
        return '';
      }
    }

    function setFieldValue(field_key, row, value) {
      var index = matrix_index[field_key];
      if (index) {
        var selector = "input[name='submitted[group_registration_table][" + row + "][" + index + "]']";
        // console.log("setting " + selector + " to " + value);
        $(selector).val(value);
      }
    }

    function processMatrixFieldChange(e) {
      if (e && e.target.name.match(/^submitted\[group_registration_table\]\[\d+\]\[\d+\]$/)) {
        var ids = e.target.name.match(/\[\d+\]\[\d+\]$/)[0].split(/[\[\]]/);
        var row = ids[1];
        var index = ids[3];
        // console.log("row: " + row + ", index: " + index);
        if (matrix_triggers[index]) {
          var prefix = matrix_triggers[index];
          var formal_title = getFieldValue(prefix + 'formal_title', row);
          var first_name   = getFieldValue(prefix + 'first_name', row);
          var last_name    = getFieldValue(prefix + 'last_name', row);

          var badge_name = formal_title + " " + first_name + " " + last_name;
          setFieldValue(prefix + 'badge', row, badge_name.trim());
        }
      }

      // also, just fill all the (remaining) badge fields
      // DISABLED: see ICA-5245
      // var organization_badge = Drupal.settings.ica_event_cmrf_connector.variables.organization_badge;
      // $("input[name$='][" + matrix_index['registrant_organisation_badge'] + "]']").each(function() {
      //   var current_value = $(this).val();
      //   if (!current_value || current_value.length == 0) {
      //     $(this).val(organization_badge);
      //   }
      // });
    }

    // catch ALL change events on table for processing
    $("body").change(processMatrixFieldChange);
    $(document).ready(processMatrixFieldChange(null));
  });
})(jQuery);


