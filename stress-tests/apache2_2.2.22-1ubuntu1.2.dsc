-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: apache2
Binary: apache2.2-common, apache2.2-bin, apache2-mpm-worker, apache2-mpm-prefork, apache2-mpm-event, apache2-mpm-itk, apache2-utils, apache2-suexec, apache2-suexec-custom, apache2, apache2-doc, apache2-prefork-dev, apache2-threaded-dev, apache2-dbg
Architecture: any all
Version: 2.2.22-1ubuntu1.2
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Stefan Fritsch <sf@debian.org>, Steinar H. Gunderson <sesse@debian.org>, Arno Töll <debian@toell.net>
Homepage: http://httpd.apache.org/
Standards-Version: 3.9.2
Vcs-Bzr: http://code.launchpad.net/ubuntu/+source/apache2
Build-Depends: debhelper (>= 8), lsb-release, libaprutil1-dev (>= 1.3.4), libapr1-dev, openssl, libpcre3-dev, mawk, zlib1g-dev, libssl-dev (>= 0.9.8m), sharutils, libcap-dev [linux-any], autoconf, hardening-wrapper
Build-Conflicts: autoconf2.13
Package-List: 
 apache2 deb httpd optional
 apache2-dbg deb debug extra
 apache2-doc deb doc optional
 apache2-mpm-event deb httpd optional
 apache2-mpm-itk deb httpd extra
 apache2-mpm-prefork deb httpd optional
 apache2-mpm-worker deb httpd optional
 apache2-prefork-dev deb httpd extra
 apache2-suexec deb httpd optional
 apache2-suexec-custom deb httpd extra
 apache2-threaded-dev deb httpd extra
 apache2-utils deb httpd optional
 apache2.2-bin deb httpd optional
 apache2.2-common deb httpd optional
Checksums-Sha1: 
 bf3bbfda967ac900348e697f26fe86b25695efe9 7200529 apache2_2.2.22.orig.tar.gz
 3664685c83ffa736a0b7c2a9865beb51d16d14d8 216844 apache2_2.2.22-1ubuntu1.2.debian.tar.gz
Checksums-Sha256: 
 74c1ffffefe1a502339b004ad6488fbd858eb425a05968cd67c05695dbc0fe7c 7200529 apache2_2.2.22.orig.tar.gz
 055be6d79b7e0a598d8199c785e406f4e61fdeb100889cffb25ffd9118f6d79e 216844 apache2_2.2.22-1ubuntu1.2.debian.tar.gz
Files: 
 d77fa5af23df96a8af68ea8114fa6ce1 7200529 apache2_2.2.22.orig.tar.gz
 fa8fa5c50f7a6efb07ddacb58154f0cf 216844 apache2_2.2.22-1ubuntu1.2.debian.tar.gz
Original-Maintainer: Debian Apache Maintainers <debian-apache@lists.debian.org>
Original-Vcs-Browser: http://anonscm.debian.org/gitweb/?p=pkg-apache/apache2.git
Original-Vcs-Git: git://git.debian.org/git/pkg-apache/apache2.git

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1.4.11 (GNU/Linux)

iQIcBAEBCgAGBQJQnCCFAAoJEGVp2FWnRL6Tl88P/jI4BjLYxzxcb6kvtV01ThR6
wgSYh4NbEIl9R8Oz51JTKItT8DAXzDQioZgmPRL7zmpEbDGo6jOeSyxQS568vLuf
UuQUEFZnWmzAHUZ3vkF5AFxaQzI2qlSIq3ef//U8+yP5AGWiQeuQJDWzVU3iiRdz
NM0SLWBYhqBNjKWlBUXFRwLZEoGYPFeGZbfiS6vtQp73Fo95OK/SPTP1ZHBHJU+H
bT7iQZNmW4sf61wry8rUrvxEaYMj0wpJREgRng31kgaAGz6AMmhP5sz0XnZaLvAb
A9ew1366W18EeA8lNkkqgp5V0FOlVjo+ogkTNqIR+1CDCxNFy1vXPzYMHp7cm7SD
yi8LYm111GRaAmr/2f2AKcQnCWD9+pLweqKaUPE5rhTb7evd7ODZqHa/Z1cFNuet
z10X75Lu8WoYFYir5z3ctFd4ITBHzzfvef+bk/RqPvJ0o6MLyTk+pEleglR/3NOr
CoQjtEJfoiRPQ4soPzjyd9TIl2q6ZkZewFMypTL3WgHqKZzVMSmGW4fCd498MV0D
QqrJPALz4eCov69kZyFrwdbPvaA4I8T+I895Z+Vz4brm3pan0RZLBYhbo6Cfrvzh
/ou07IOBItwet+qXYPtSE6U97lZc+yfi1FuEFmBnLu78TbP35wMGFaoIbwdklxh4
4BJolmOnfKj9vwFssdrd
=UOWk
-----END PGP SIGNATURE-----
