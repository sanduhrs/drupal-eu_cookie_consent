(function($) {

  var once = true;
  Drupal.behaviors.EUCookieConsent = {
    attach: function (context, settings) {
      if (once) {
        if (typeof($.EUCookie) !== 'undefined') {
          // Inspired on https://www.lullabot.com/articles/your-javascript-should-expose-apis-too
          var opts = Drupal.settings.EUCookieConsent;
          $(document).trigger('EUCookie.init', opts);
          $.EUCookie(opts).init();
          once = false;
        }
      }
    }
  };
})(jQuery);
