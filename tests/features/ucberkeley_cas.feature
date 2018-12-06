Feature:
  As a site builder
  In order to control user content creation priviliges
  I need to be able to authenticate via CalNet

  Background:
    Given the "ucberkeley_cas" feature is enabled
    And I am an anonymous user

  Scenario:
    When I go to "cas"
    Then the url should match "/cas/login"
