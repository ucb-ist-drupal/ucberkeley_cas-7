api = 2
core = 7.x

; UCB CAS
;projects[ucb_cas][type] = "module"
;projects[ucb_cas][download][type] = "git"
;; git@github-ucbdrupal.com is for bwood's laptop, others would use git@github.com
;projects[ucb_cas][download][url] = "git@github-ucbdrupal.com:ucbdrupal/ucb_cas.git"
;projects[ucb_cas][download][branch] = "7.x-1.x"
;; [download][tag] not working, use revision for now.
;projects[ucb_cas][download][revision] = "c0048f2ad0df4be69ec8a692bef58901685bfdf3"

; CAS
projects[cas] = 1.2
projects[cas][patch][1394666-cas_library_path-5.patch] = "http://drupal.org/files/1394666-cas_library_path-5.patch"
projects[cas][subdir] = "ucb_cas"

; phpCAS library
libraries[cas][download][type] = "get"
libraries[cas][download][url] = "http://downloads.jasig.org/cas-clients/php/current/CAS-1.3.1.tgz"
libraries[cas][destination] = "modules/ucb_cas/cas"

libraries[cas][directory_name] = "CAS"

; CAS Attributes
projects[cas_attributes] = 1.0-beta2 
projects[cas_attributes][subdir] = "ucb_cas"

; LDAP
projects[ldap] = 1.0-beta10
projects[ldap][subdir] = "ucb_cas"

