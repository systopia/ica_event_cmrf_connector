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

    function setBatchName() {
      var title      = $("input[name='submitted[registrant_title]']").val();
      var first_name = $("input[name='submitted[registrant_first_name]']").val();
      var last_name  = $("input[name='submitted[registrant_last_name]']").val();
      var badge_name = title + " " + first_name + " " + last_name;

      $("input[name='submitted[registrant_badge]']").val(badge_name.trim());
    }

    $("input[name='submitted[registrant_title]']").change(setBatchName);
    $("input[name='submitted[registrant_first_name]']").change(setBatchName);
    $("input[name='submitted[registrant_last_name]']").change(setBatchName);
  });
})(jQuery);
