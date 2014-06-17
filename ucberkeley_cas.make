api = 2
core = 7.x

; phpCAS library
libraries[phpcas][download][type] = "get"
libraries[phpcas][download][url] = "http://downloads.jasig.org/cas-clients/php/current/CAS-1.3.2.tgz"

; CAS
projects[cas][type] = module
projects[cas][version] = 1.3
projects[cas][patch][1394666-cas_library_path-15.patch] = "https://drupal.org/files/cas-library-detection-1394666-15.patch"

; CAS Attributes
projects[cas_attributes][type] = module
projects[cas_attributes][version] = 1.0-beta2

; LDAP
projects[ldap][type] = module
projects[ldap][version] = 1.0-beta12

; ****************************
; *****Panopoly Features *****

; Use Drush 6 to run make file. See https://github.com/drush-ops/drush/issues/15

; Previously, makefiles were parsed bottom-up, and that in Drush concurrency might
; interfere with recursion.
; Therefore PANOPOLY needs to be listed AT THE BOTTOM of this makefile,
; so we can patch or update certain projects fetched by Panopoly's makefiles.

; The Panopoly Foundation

projects[panopoly_core][version] = 1.5
projects[panopoly_core][subdir] = panopoly

projects[panopoly_images][version] = 1.5
projects[panopoly_images][subdir] = panopoly

projects[panopoly_theme][version] = 1.5
projects[panopoly_theme][subdir] = panopoly

projects[panopoly_magic][version] = 1.5
projects[panopoly_magic][subdir] = panopoly

projects[panopoly_widgets][version] = 1.5
projects[panopoly_widgets][subdir] = panopoly

projects[panopoly_admin][version] = 1.5
projects[panopoly_admin][subdir] = panopoly

projects[panopoly_users][version] = 1.5
projects[panopoly_users][subdir] = panopoly

; The Panopoly Toolset

projects[panopoly_pages][version] = 1.5
projects[panopoly_pages][subdir] = panopoly

projects[panopoly_wysiwyg][version] = 1.5
projects[panopoly_wysiwyg][subdir] = panopoly

projects[panopoly_search][version] = 1.5
projects[panopoly_search][subdir] = panopoly
