#!/usr/bin/python

'''apport hook for apache2

(c) 2010 Adam Sommer.
Author: Adam Sommer <asommer@ubuntu.com>

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.  See http://www.gnu.org/copyleft/gpl.html for
the full text of the license.
'''

from apport.hookutils import *
import os

def add_info(report, ui):
    response = ui.yesno("The contents of your /etc/apache2/sites-enabled directory "
                        "may help developers diagnose your bug more "
                        "quickly.  However, it may contain sensitive "
                        "information.  Do you want to include it in your "
                        "bug report?")

    if response == None: # user cancelled
        raise StopIteration

    elif response == True:
        # Attache config files in /etc/apache2/sites-enabled and listing of files in /etc/apache2/conf.d 
        for conf_file in os.listdir('/etc/apache2/sites-enabled'):
            attach_file_if_exists(report, '/etc/apache2/sites-enabled/' + conf_file, conf_file)
        try:
            report['Apache2ConfdDirListing'] = unicode(os.listdir('/etc/apache2/conf.d'))
        except OSError:
            report['Apache2ConfdDirListing'] = unicode(False)

        # Attach default config files if changed.
        attach_conffiles(report, 'apache2.2-common', conffiles=None)

        # Attach the error.log file.
        attach_file(report, '/var/log/apache2/error.log', key='error.log')

        # Get loaded modules.
        report['Apache2Modules'] = root_command_output(['/usr/sbin/apachectl', '-D DUMP_MODULES'])
