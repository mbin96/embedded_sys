mod_negotiation.la: mod_negotiation.lo
	$(MOD_LINK) mod_negotiation.lo $(MOD_NEGOTIATION_LDADD)
mod_dir.la: mod_dir.lo
	$(MOD_LINK) mod_dir.lo $(MOD_DIR_LDADD)
mod_imap.la: mod_imap.lo
	$(MOD_LINK) mod_imap.lo $(MOD_IMAP_LDADD)
mod_actions.la: mod_actions.lo
	$(MOD_LINK) mod_actions.lo $(MOD_ACTIONS_LDADD)
mod_userdir.la: mod_userdir.lo
	$(MOD_LINK) mod_userdir.lo $(MOD_USERDIR_LDADD)
mod_alias.la: mod_alias.lo
	$(MOD_LINK) mod_alias.lo $(MOD_ALIAS_LDADD)
mod_so.la: mod_so.lo
	$(MOD_LINK) mod_so.lo $(MOD_SO_LDADD)
DISTCLEAN_TARGETS = modules.mk
static =  mod_negotiation.la mod_dir.la mod_imap.la mod_actions.la mod_userdir.la mod_alias.la mod_so.la
shared = 
