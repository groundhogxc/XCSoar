from os.path import abspath

from build.zlib import ZlibProject
from build.autotools import AutotoolsProject
from build.openssl import OpenSSLProject
from build.freetype import FreeTypeProject
from build.sdl2 import SDL2Project
from build.lua import LuaProject

glibc = AutotoolsProject(
    'http://mirror.netcologne.de/gnu/libc/glibc-2.23.tar.xz',
    '456995968f3acadbed39f5eba31678df',
    'include/unistd.h',
    [
        '--enable-static-nss',
        '--enable-kernel=2.6.35',
        '--disable-werror',
        '--disable-build-nscd',
        '--disable-nscd',
    ],
    shared=True,
)

openssl = OpenSSLProject(
    'https://openssl.org/source/openssl-1.0.2g.tar.gz',
    'f3c710c045cdee5fd114feb69feba7aa',
    'include/openssl/ossl_typ.h',
)

openssh = AutotoolsProject(
    'http://ftp.openbsd.org/pub/OpenBSD/OpenSSH/portable/openssh-7.2p2.tar.gz',
    '13009a9156510d8f27e752659075cced',
    'opt/openssh/sbin/sshd',
    [
        '--disable-etc-default-login',
        '--disable-lastlog',
        '--disable-utmp',
        '--disable-utmpx',
        '--disable-wtmp',
        '--disable-wtmpx',
        '--disable-libutil',
        '--disable-pututline',
        '--disable-pututxline',
        '--without-openssl',
        '--without-ssh1',
        '--without-stackprotect',
        '--without-hardening',
        '--without-shadow',
        '--without-sandbox',
        '--without-selinux',
    ],
    ldflags='-static',
    install_prefix='/opt/openssh',
    install_target='install-nosysconf',
    use_destdir=True,
)

zlib = ZlibProject(
    'http://zlib.net/zlib-1.2.8.tar.xz',
    '28f1205d8dd2001f26fec1e8c2cebe37',
    'lib/libz.a',
)

freetype = FreeTypeProject(
    'http://download.savannah.gnu.org/releases/freetype/freetype-2.6.3.tar.bz2',
    '0037b25a8c090bc8a1218e867b32beb1',
    'lib/libfreetype.a',
    [
        '--disable-shared', '--enable-static',
        '--without-bzip2', '--without-png',
        '--without-harfbuzz',
    ],
)

curl = AutotoolsProject(
    'http://curl.haxx.se/download/curl-7.48.0.tar.lzma',
    'de75665721e3adb0a1d7e03b90c3d7a2',
    'lib/libcurl.a',
    [
        '--disable-shared', '--enable-static',
        '--disable-debug',
        '--enable-http',
        '--enable-ipv6',
        '--disable-ftp', '--disable-file',
        '--disable-ldap', '--disable-ldaps',
        '--disable-rtsp', '--disable-proxy', '--disable-dict', '--disable-telnet',
        '--disable-tftp', '--disable-pop3', '--disable-imap', '--disable-smb',
        '--disable-smtp',
        '--disable-gopher',
        '--disable-manual',
        '--disable-threaded-resolver', '--disable-verbose', '--disable-sspi',
        '--disable-crypto-auth', '--disable-ntlm-wb', '--disable-tls-srp', '--disable-cookies',
        '--without-ssl', '--without-gnutls', '--without-nss', '--without-libssh2',
    ],
    use_clang=True,
)

proj = AutotoolsProject(
    'http://download.osgeo.org/proj/proj-4.9.2.tar.gz',
    '9843131676e31bbd903d60ae7dc76cf9',
    'lib/libproj.a',
    [
        '--disable-shared', '--enable-static',
        '--without-mutex',
    ],
    patches=abspath('lib/proj/patches'),
    autogen=True,
)

libpng = AutotoolsProject(
    'ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng16/libpng-1.6.21.tar.xz',
    '3bacb4728f6694a64ad9052769d6a4ce',
    'lib/libpng.a',
    [
        '--disable-shared', '--enable-static',
        '--enable-arm-neon',
    ]
)

libjpeg = AutotoolsProject(
    'http://downloads.sourceforge.net/project/libjpeg-turbo/1.4.2/libjpeg-turbo-1.4.2.tar.gz',
    '86b0d5f7507c2e6c21c00219162c3c44',
    'lib/libjpeg.a',
    [
        '--disable-shared', '--enable-static',
    ]
)

libtiff = AutotoolsProject(
    'http://download.osgeo.org/libtiff/tiff-4.0.6.tar.gz',
    'd1d2e940dea0b5ad435f21f03d96dd72',
    'lib/libtiff.a',
    [
        '--disable-shared', '--enable-static',
        '--disable-largefile',
        '--disable-cxx',
        '--disable-ccitt',
        '--disable-packbits',
        '--disable-lzw',
        '--disable-thunder',
        '--disable-next',
        '--disable-logluv',
        '--disable-mdi',
        '--disable-pixarlog',
        '--disable-jpeg',
        '--disable-old-jpeg',
        '--disable-jbig',
        '--disable-lzma',
        '--disable-strip-chopping',
        '--disable-extrasample-as-alpha',
    ],
    base='tiff-4.0.6',
    patches=abspath('lib/libtiff/patches'),
    autogen=True,
)

libgeotiff = AutotoolsProject(
    'http://download.osgeo.org/geotiff/libgeotiff/libgeotiff-1.4.1.tar.gz',
    '48bdf817e6e7a37671cc1f41b01e10fc',
    'lib/libgeotiff.a',
    [
        '--disable-shared', '--enable-static',
        '--disable-doxygen-doc',
        '--disable-doxygen-dot',
        '--disable-doxygen-man',
        '--disable-doxygen-html',
    ],
    patches=abspath('lib/libgeotiff/patches'),
    autogen=True,
    libs='-lz',
)

sdl2 = SDL2Project(
    'http://www.libsdl.org/tmp/release/SDL2-2.0.4.tar.gz',
    '20d214724eb991787e34f43a6c822f67',
    'lib/libSDL2.a',
    [
        '--disable-shared', '--enable-static',
    ]
)

lua = LuaProject(
    'http://www.lua.org/ftp/lua-5.3.2.tar.gz',
    '33278c2ab5ee3c1a875be8d55c1ca2a1',
    'lib/liblua.a',
    patches=abspath('lib/lua/patches'),
)
