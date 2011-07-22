
Drupal.behaviors.cas = function (context) {
  var $loginElements = $("#edit-name-wrapper, #edit-pass-wrapper, li.cas-link, li.openid-link");
  var $casElements = $("li.uncas-link, .cas-login-redirection-message");
  $("#edit-cas-identifier-wrapper").hide();
  if($("#edit-cas-identifier").attr("checked")) {
    $loginElements.hide();
    // Use .css("display", "block") instead of .show() to be Konqueror friendly.
    $casElements.css("display", "block");
  }
  else
  {
    $loginElements.css("display", "block");
    // Use .css("display", "block") instead of .show() to be Konqueror friendly.
    $casElements.hide();
  }

  $("li.cas-link", context)
    .click( function() {
      $loginElements.hide();
      $casElements.css("display", "block");
      $("#edit-cas-identifier").attr("checked", true);
      // Remove possible error message.
      $("#edit-name, #edit-pass").removeClass("error");
      $("div.messages.error").hide();
      return false;
    });
  $("li.uncas-link", context)
    .click(function() {
      $loginElements.css("display", "block");
      // Fix OpenID compatibility.
      $('li.user-link').hide();
      $casElements.hide();
      $("#edit-cas-identifier").attr("checked", false);
      // Clear cas Identifier field and remove possible error message.
      $("div.messages.error").css("display", "block");
      // Set focus on username field.
      $("#edit-name")[0].focus();
      return false;
    });
  // OpenID Compatibility
  $("li.openid-link", context)
    .click( function() {
      $("li.cas-link").hide();
    });
  $("li.user-link", context)
    .click( function() {
      $("li.cas-link").css("display", "block");
    });
};
