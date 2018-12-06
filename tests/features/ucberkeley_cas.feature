Feature:
  As a site builder
  In order to control user content creation priviliges
  I need to be able to authenticate via CalNet

  Background:
    Given the "ucberkeley_cas" feature is enabled
    And I am an anonymous user
    And I am on the homepage

  Scenario:
    When I go to "cas"
    Then the url should match "/cas/login"

  Scenario:
    When I go to "user/login"
    Then I should see "You will be redirected to the secure CalNet login page."
    When I press the "Log in" button
    Then the url should match "/cas/login"

  Scenario:
    When I go to "user/admin_login"
    Then I should see "Admin Login"
    And I should see "Username"
    And I should see "Password"
    And I should see the button "Log in"

