libmod_status.la: mod_status.lo
	$(MOD_LINK) mod_status.lo $(MOD_STATUS_LDADD)
libmod_autoindex.la: mod_autoindex.lo
	$(MOD_LINK) mod_autoindex.lo $(MOD_AUTOINDEX_LDADD)
libmod_asis.la: mod_asis.lo
	$(MOD_LINK) mod_asis.lo $(MOD_ASIS_LDADD)
libmod_cgi.la: mod_cgi.lo
	$(MOD_LINK) mod_cgi.lo $(MOD_CGI_LDADD)
DISTCLEAN_TARGETS = modules.mk
static =  libmod_status.la libmod_autoindex.la libmod_asis.la libmod_cgi.la
shared = 
