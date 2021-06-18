api = 2
core = 7.x

; phpCAS library
libraries[phpcas][download][type] = "get"
;; cas 7.x-1.7 generates many warnings with phpCAS 1.4.0 so we are staying on 1.3.8
;; https://jira-secure.berkeley.edu/browse/OPENUCB-2421?focusedCommentId=1641203&page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel#comment-1641203
libraries[phpcas][download][url] = "https://github.com/Jasig/phpCAS/archive/1.3.8.tar.gz"

; CAS
projects[cas][type] = module
projects[cas][version] = 1.7
projects[cas][patch][1394666] = "https://drupal.org/files/cas-library-detection-1394666-15.patch"

; CAS Attributes
projects[cas_attributes][type] = module
projects[cas_attributes][version] = 1.0-rc3
; fix single quotes in field data
projects[cas_attributes][patch][3031238] = https://www.drupal.org/files/issues/2021-02-03/cas_attributes-special-characters-3031238-9.patch

; LDAP
projects[ldap][type] = module
projects[ldap][version] = 2.6
projects[ldap][patch][patchforkey] = "https://github.com/CellarDoorMedia/Lockr-Patches/raw/master/drupal7/ldap/ldap-7.x-2.5-key-integration.patch"

; Realname
projects[realname][type] = module
projects[realname][version] = 1.4
projects[realname][patch][2926684] = https://www.drupal.org/files/issues/realname-views-autocomplete-2926684-2.patch

; UC Berkeley CAS Feature
; TODO: update dl options
; specify type=module to prevent "No release history was found for the requested project (ucberkeley_cas)."
projects[ucberkeley_cas][type] = "module"
projects[ucberkeley_cas][download][type] = "git"
projects[ucberkeley_cas][download][url] = "git@github.com:bwood/ucberkeley_cas-7.git"
projects[ucberkeley_cas][download][branch] = "master"
projects[ucberkeley_cas][download][tag] = "7.x-5.1.2"
