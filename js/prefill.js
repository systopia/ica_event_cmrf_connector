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


(function($) {
  $(document).ready(function(){

    function setRegistrantBatchName() {
      setBatchName("registrant");
    }

    function setPartnerBatchName() {
      setBatchName("partner");
    }

    function setBatchName(prefix) {
      var title      = $("input[name='submitted[" + prefix + "_title]']").val();
      var first_name = $("input[name='submitted[" + prefix + "_first_name]']").val();
      var last_name  = $("input[name='submitted[" + prefix + "_last_name]']").val();
      var badge_name = title + " " + first_name + " " + last_name;

      $("input[name='submitted[" + prefix + "_badge]']").val(badge_name.trim());
    }

    $("input[name='submitted[registrant_title]']").change(setRegistrantBatchName);
    $("input[name='submitted[registrant_first_name]']").change(setRegistrantBatchName);
    $("input[name='submitted[registrant_last_name]']").change(setRegistrantBatchName);
    $("input[name='submitted[partner_title]']").change(setPartnerBatchName);
    $("input[name='submitted[partner_first_name]']").change(setPartnerBatchName);
    $("input[name='submitted[partner_last_name]']").change(setPartnerBatchName);
  });
})(jQuery);
