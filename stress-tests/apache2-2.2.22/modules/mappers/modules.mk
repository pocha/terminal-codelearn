libmod_negotiation.la: mod_negotiation.lo
	$(MOD_LINK) mod_negotiation.lo $(MOD_NEGOTIATION_LDADD)
libmod_dir.la: mod_dir.lo
	$(MOD_LINK) mod_dir.lo $(MOD_DIR_LDADD)
libmod_actions.la: mod_actions.lo
	$(MOD_LINK) mod_actions.lo $(MOD_ACTIONS_LDADD)
libmod_userdir.la: mod_userdir.lo
	$(MOD_LINK) mod_userdir.lo $(MOD_USERDIR_LDADD)
libmod_alias.la: mod_alias.lo
	$(MOD_LINK) mod_alias.lo $(MOD_ALIAS_LDADD)
libmod_so.la: mod_so.lo
	$(MOD_LINK) mod_so.lo $(MOD_SO_LDADD)
DISTCLEAN_TARGETS = modules.mk
static =  libmod_negotiation.la libmod_dir.la libmod_actions.la libmod_userdir.la libmod_alias.la libmod_so.la
shared = 
