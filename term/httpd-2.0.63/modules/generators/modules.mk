mod_status.la: mod_status.lo
	$(MOD_LINK) mod_status.lo $(MOD_STATUS_LDADD)
mod_autoindex.la: mod_autoindex.lo
	$(MOD_LINK) mod_autoindex.lo $(MOD_AUTOINDEX_LDADD)
mod_asis.la: mod_asis.lo
	$(MOD_LINK) mod_asis.lo $(MOD_ASIS_LDADD)
mod_cgi.la: mod_cgi.lo
	$(MOD_LINK) mod_cgi.lo $(MOD_CGI_LDADD)
DISTCLEAN_TARGETS = modules.mk
static =  mod_status.la mod_autoindex.la mod_asis.la mod_cgi.la
shared = 
