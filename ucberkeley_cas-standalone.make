api = 2
core = 7.x

; phpCAS library
libraries[phpcas][download][type] = "get"
libraries[phpcas][download][url] = "https://github.com/Jasig/phpCAS/archive/1.3.8.tar.gz"

; CAS
projects[cas][type] = module
projects[cas][version] = 1.7
projects[cas][patch][1394666] = "https://drupal.org/files/cas-library-detection-1394666-15.patch"

; CAS Attributes
projects[cas_attributes][type] = module
projects[cas_attributes][version] = 1.0-rc3
; fix single quotes in field data
projects[cas_attributes][patch][3031238] = https://www.drupal.org/files/issues/2019-02-06/cas_attributes-single-quotes-3031238-4.patch

; LDAP
projects[ldap][type] = module
projects[ldap][version] = 2.5
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
projects[ucberkeley_cas][download][tag] = "7.x-5.1.0"
