QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += static
QTPLUGIN += qsqloci qgif
DEFINES += STATIC
CONFIG += qt static
QMAKE_LFLAGS += -static-libgcc -static-libstdc++


SOURCES += \
    decryption.cpp \
    encryption.cpp \
    keygeneration.cpp \
    main.cpp \
    menu.cpp

HEADERS += \
    includes/base64.h \
    cryptopp/3way.h \
    cryptopp/adler32.h \
    cryptopp/adv_simd.h \
    cryptopp/aes.h \
    cryptopp/aes_armv4.h \
    cryptopp/algebra.h \
    cryptopp/algparam.h \
    cryptopp/allocate.h \
    cryptopp/arc4.h \
    cryptopp/argnames.h \
    cryptopp/aria.h \
    cryptopp/arm_simd.h \
    cryptopp/asn.h \
    cryptopp/authenc.h \
    cryptopp/base32.h \
    cryptopp/base64.h \
    cryptopp/basecode.h \
    cryptopp/bench.h \
    cryptopp/blake2.h \
    cryptopp/blowfish.h \
    cryptopp/blumshub.h \
    cryptopp/camellia.h \
    cryptopp/cast.h \
    cryptopp/cbcmac.h \
    cryptopp/ccm.h \
    cryptopp/chacha.h \
    cryptopp/chachapoly.h \
    cryptopp/cham.h \
    cryptopp/channels.h \
    cryptopp/cmac.h \
    cryptopp/config.h \
    cryptopp/config_align.h \
    cryptopp/config_asm.h \
    cryptopp/config_cpu.h \
    cryptopp/config_cxx.h \
    cryptopp/config_dll.h \
    cryptopp/config_int.h \
    cryptopp/config_misc.h \
    cryptopp/config_ns.h \
    cryptopp/config_os.h \
    cryptopp/config_ver.h \
    cryptopp/cpu.h \
    cryptopp/crc.h \
    cryptopp/cryptlib.h \
    cryptopp/darn.h \
    cryptopp/default.h \
    cryptopp/des.h \
    cryptopp/dh.h \
    cryptopp/dh2.h \
    cryptopp/dll.h \
    cryptopp/dmac.h \
    cryptopp/donna.h \
    cryptopp/donna_32.h \
    cryptopp/donna_64.h \
    cryptopp/donna_sse.h \
    cryptopp/drbg.h \
    cryptopp/dsa.h \
    cryptopp/eax.h \
    cryptopp/ec2n.h \
    cryptopp/eccrypto.h \
    cryptopp/ecp.h \
    cryptopp/ecpoint.h \
    cryptopp/elgamal.h \
    cryptopp/emsa2.h \
    cryptopp/eprecomp.h \
    cryptopp/esign.h \
    cryptopp/factory.h \
    cryptopp/fhmqv.h \
    cryptopp/files.h \
    cryptopp/filters.h \
    cryptopp/fips140.h \
    cryptopp/fltrimpl.h \
    cryptopp/gcm.h \
    cryptopp/gf256.h \
    cryptopp/gf2_32.h \
    cryptopp/gf2n.h \
    cryptopp/gfpcrypt.h \
    cryptopp/gost.h \
    cryptopp/gzip.h \
    cryptopp/hashfwd.h \
    cryptopp/hc128.h \
    cryptopp/hc256.h \
    cryptopp/hex.h \
    cryptopp/hight.h \
    cryptopp/hkdf.h \
    cryptopp/hmac.h \
    cryptopp/hmqv.h \
    cryptopp/hrtimer.h \
    cryptopp/ida.h \
    cryptopp/idea.h \
    cryptopp/integer.h \
    cryptopp/iterhash.h \
    cryptopp/kalyna.h \
    cryptopp/keccak.h \
    cryptopp/lea.h \
    cryptopp/lsh.h \
    cryptopp/lubyrack.h \
    cryptopp/luc.h \
    cryptopp/mars.h \
    cryptopp/md2.h \
    cryptopp/md4.h \
    cryptopp/md5.h \
    cryptopp/mdc.h \
    cryptopp/mersenne.h \
    cryptopp/misc.h \
    cryptopp/modarith.h \
    cryptopp/modes.h \
    cryptopp/modexppc.h \
    cryptopp/mqueue.h \
    cryptopp/mqv.h \
    cryptopp/naclite.h \
    cryptopp/nbtheory.h \
    cryptopp/nr.h \
    cryptopp/oaep.h \
    cryptopp/oids.h \
    cryptopp/osrng.h \
    cryptopp/ossig.h \
    cryptopp/padlkrng.h \
    cryptopp/panama.h \
    cryptopp/pch.h \
    cryptopp/pem.h \
    cryptopp/pem_common.h \
    cryptopp/pkcspad.h \
    cryptopp/poly1305.h \
    cryptopp/polynomi.h \
    cryptopp/ppc_simd.h \
    cryptopp/pssr.h \
    cryptopp/pubkey.h \
    cryptopp/pwdbased.h \
    cryptopp/queue.h \
    cryptopp/rabbit.h \
    cryptopp/rabin.h \
    cryptopp/randpool.h \
    cryptopp/rc2.h \
    cryptopp/rc5.h \
    cryptopp/rc6.h \
    cryptopp/rdrand.h \
    cryptopp/resource.h \
    cryptopp/rijndael.h \
    cryptopp/ripemd.h \
    cryptopp/rng.h \
    cryptopp/rsa.h \
    cryptopp/rw.h \
    cryptopp/safer.h \
    cryptopp/salsa.h \
    cryptopp/scrypt.h \
    cryptopp/seal.h \
    cryptopp/secblock.h \
    cryptopp/secblockfwd.h \
    cryptopp/seckey.h \
    cryptopp/seed.h \
    cryptopp/serpent.h \
    cryptopp/serpentp.h \
    cryptopp/sha.h \
    cryptopp/sha1_armv4.h \
    cryptopp/sha256_armv4.h \
    cryptopp/sha3.h \
    cryptopp/sha512_armv4.h \
    cryptopp/shacal2.h \
    cryptopp/shake.h \
    cryptopp/shark.h \
    cryptopp/simeck.h \
    cryptopp/simon.h \
    cryptopp/simple.h \
    cryptopp/siphash.h \
    cryptopp/skipjack.h \
    cryptopp/sm3.h \
    cryptopp/sm4.h \
    cryptopp/smartptr.h \
    cryptopp/sosemanuk.h \
    cryptopp/speck.h \
    cryptopp/square.h \
    cryptopp/stdcpp.h \
    cryptopp/strciphr.h \
    cryptopp/tea.h \
    cryptopp/threefish.h \
    cryptopp/tiger.h \
    cryptopp/trap.h \
    cryptopp/trunhash.h \
    cryptopp/ttmac.h \
    cryptopp/tweetnacl.h \
    cryptopp/twofish.h \
    cryptopp/validate.h \
    cryptopp/vmac.h \
    cryptopp/wake.h \
    cryptopp/whrlpool.h \
    cryptopp/words.h \
    cryptopp/x509cert.h \
    cryptopp/xed25519.h \
    cryptopp/xtr.h \
    cryptopp/xtrcrypt.h \
    cryptopp/xts.h \
    cryptopp/zdeflate.h \
    cryptopp/zinflate.h \
    cryptopp/zlib.h \
    decryption.h \
    encryption.h \
    includes/gmp.h \
    includes/gmpxx.h \
    keygeneration.h \
    menu.h

FORMS += \
    decryption.ui \
    encryption.ui \
    keygeneration.ui \
    menu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -lgmpxx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -lgmpxxd

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/libgmpxx.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/libgmpxxd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/gmpxx.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/gmpxxd.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -lgmp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -lgmpd
else:unix: LIBS += -L$$PWD/libs/ -lgmp

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/libgmp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/libgmpd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/gmp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/gmpd.lib
else:unix: PRE_TARGETDEPS += $$PWD/libs/libgmp.a

DISTFILES += \
    icons/cross.png \
    icons/home.png \
    icons/tick.png

RESOURCES += \
    resources.qrc


INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -lcryptopp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -lcryptoppd

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -lcryptopp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -lcryptoppd

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/libcryptopp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/libcryptoppd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/cryptopp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/cryptoppd.lib

