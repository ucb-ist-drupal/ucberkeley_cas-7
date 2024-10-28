api = 2
core = 7.x

; phpCAS library
;; This is now installed by composer in rebuild.sh

; CAS
projects[cas][type] = module
projects[cas][version] = 1.8
projects[cas][patch][1394666] = "https://drupal.org/files/cas-library-detection-1394666-15.patch"

; CAS Attributes
projects[cas_attributes][type] = module
projects[cas_attributes][version] = 1.0-rc3
; fix single quotes in field data
projects[cas_attributes][patch][3031238] = https://www.drupal.org/files/issues/2021-02-03/cas_attributes-special-characters-3031238-9.patch
projects[cas_attributes][patch][1] = patches/cas_ldap-reuse-ldap-connection.patch

; LDAP
projects[ldap][type] = module
projects[ldap][version] = 2.7
projects[ldap][download][type] = file
projects[ldap][download][url] = https://gitlab.com/api/v4/projects/55398792/packages/generic/ldap/7.x-2.7/ldap-7.x-2.7.tar.gz
projects[ldap][patch][patchforkey] = "https://github.com/CellarDoorMedia/Lockr-Patches/raw/master/drupal7/ldap/ldap-7.x-2.5-key-integration.patch"
projects[ldap][patch][3302242] = "patches/ldap-php-8-compatibility-3302242-new-modified.patch"

; Realname
projects[realname][type] = module
projects[realname][version] = 1.4
projects[realname][patch][2926684] = https://www.drupal.org/files/issues/realname-views-autocomplete-2926684-2.patch
projects[realname][patch][3263690] = https://www.drupal.org/files/issues/2022-03-17/realname_autocomplete_array_offset_warning-3263690-2.patch

; UC Berkeley CAS Feature
; specify type=module to prevent "No release history was found for the requested project (ucberkeley_cas)."
projects[ucberkeley_cas][type] = "module"
projects[ucberkeley_cas][download][type] = "git"
; You might need to update this url to reflect your own user:
projects[ucberkeley_cas][download][url] = "git@github-bwood.com:bwood/ucberkeley_cas-7.git"
projects[ucberkeley_cas][download][branch] = "openucb-2754"
