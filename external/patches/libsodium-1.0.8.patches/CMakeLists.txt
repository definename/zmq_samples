cmake_minimum_required(VERSION 3.2.1)

project(libsodium)

option(LIBSODIUM_BUILD_SHARED_LIBRARIES "Build the library dynamically (ON/TRUE) or statically (OFF/FALSE)." OFF)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CRTLinkage)

# Remove this set with ${Libsodium_ROOT} in external project.
#set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
message(STATUS "Libsodium install prefix: " ${CMAKE_INSTALL_PREFIX})

SET(VERSION 1.0.8)
# Todo. Set this variables:
#SODIUM_LIBRARY_VERSION_MAJOR
#SODIUM_LIBRARY_VERSION_MINOR
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/src/libsodium/include/sodium/version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/src/libsodium/include/sodium/version.h)

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(INSTALL_ARCH_DIR "${CMAKE_INSTALL_PREFIX}/lib/static")
set(INSTALL_INC_ROOT_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(INSTALL_INC_SODIUM_DIR "${INSTALL_INC_ROOT_DIR}/sodium")

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/src/libsodium
	${CMAKE_CURRENT_SOURCE_DIR}/src/libsodium/include
	${CMAKE_CURRENT_SOURCE_DIR}/src/libsodium/include/sodium
	#For version.h.in to version.h
	${CMAKE_CURRENT_BINARY_DIR}/src/libsodium/include/sodium)
	
# Sources and headers
set(CRYPTO_CORE_HEADERS_ADDITIONAL
	src/libsodium/crypto_core/curve25519/ref10/base.h
	src/libsodium/crypto_core/curve25519/ref10/base2.h
	src/libsodium/crypto_core/curve25519/ref10/curve25519_ref10.h
	)
	
set(CRYPTO_GENERICHASH_HEADERS_ADDITIONAL
	src/libsodium/crypto_generichash/blake2/ref/blake2b-load-sse2.h
	src/libsodium/crypto_generichash/blake2/ref/blake2b-load-sse41.h
	src/libsodium/crypto_generichash/blake2/ref/blake2b-round.h
	src/libsodium/crypto_generichash/blake2/ref/blake2-impl.h
	)

set(CRYPTO_ONETIMEAUTH_HEADERS_ADDITIONAL
	src/libsodium/crypto_onetimeauth/poly1305/onetimeauth_poly1305.h
	src/libsodium/crypto_onetimeauth/poly1305/sse2/poly1305_sse2.h
	src/libsodium/crypto_onetimeauth/poly1305/donna/poly1305_donna64.h
	src/libsodium/crypto_onetimeauth/poly1305/donna/poly1305_donna32.h
	src/libsodium/crypto_onetimeauth/poly1305/donna/poly1305_donna.h
	)

set(CRYPTO_SCALARMULT_HEADERS_ADDITIONAL
	src/libsodium/crypto_scalarmult/curve25519/scalarmult_curve25519.h
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/consts_namespace.h
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/curve25519_sandy2x.h
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/fe.h
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/fe51.h
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/fe51_namespace.h
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/ladder.h
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/ladder_base.h
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/ladder_base_namespace.h
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/ladder_namespace.h
	src/libsodium/crypto_scalarmult/curve25519/ref10/x25519_ref10.h
	src/libsodium/crypto_scalarmult/curve25519/donna_c64/curve25519_donna_c64.h
	src/libsodium/crypto_scalarmult/curve25519/ref10/x25519_ref10.h
	)

set(CRYPTO_STREAM_HEADERS_ADDITIONAL
	src/libsodium/crypto_stream/chacha20/stream_chacha20.h
	src/libsodium/crypto_stream/chacha20/ref/stream_chacha20_ref.h
	src/libsodium/crypto_stream/chacha20/vec/stream_chacha20_vec.h
	src/libsodium/crypto_stream/aes128ctr/portable/common.h
	src/libsodium/crypto_stream/aes128ctr/portable/consts.h
	src/libsodium/crypto_stream/aes128ctr/portable/int128.h
	src/libsodium/crypto_stream/aes128ctr/portable/types.h
	)
	
set (ROOT_INCLUDES
		src/libsodium/include/sodium.h
		${CRYPTO_CORE_HEADERS_ADDITIONAL}
		${CRYPTO_GENERICHASH_HEADERS_ADDITIONAL}
		${CRYPTO_ONETIMEAUTH_HEADERS_ADDITIONAL}
		${CRYPTO_SCALARMULT_HEADERS_ADDITIONAL}
		${CRYPTO_STREAM_HEADERS_ADDITIONAL}
		${CMAKE_CURRENT_BINARY_DIR}/src/libsodium/include/sodium/version.h
		)
	
set (SODIUM_INCLUDES
		src/libsodium/include/sodium/core.h
		src/libsodium/include/sodium/crypto_aead_aes256gcm.h
		src/libsodium/include/sodium/crypto_aead_chacha20poly1305.h
		src/libsodium/include/sodium/crypto_auth.h
		src/libsodium/include/sodium/crypto_auth_hmacsha256.h
		src/libsodium/include/sodium/crypto_auth_hmacsha512.h
		src/libsodium/include/sodium/crypto_auth_hmacsha512256.h
		src/libsodium/include/sodium/crypto_box.h
		src/libsodium/include/sodium/crypto_box_curve25519xsalsa20poly1305.h
		src/libsodium/include/sodium/crypto_core_hsalsa20.h
		src/libsodium/include/sodium/crypto_core_salsa20.h
		src/libsodium/include/sodium/crypto_core_salsa2012.h
		src/libsodium/include/sodium/crypto_core_salsa208.h
		src/libsodium/include/sodium/crypto_generichash.h
		src/libsodium/include/sodium/crypto_generichash_blake2b.h
		src/libsodium/include/sodium/crypto_hash.h
		src/libsodium/include/sodium/crypto_hash_sha256.h
		src/libsodium/include/sodium/crypto_hash_sha512.h
		src/libsodium/include/sodium/crypto_int32.h
		src/libsodium/include/sodium/crypto_int64.h
		src/libsodium/include/sodium/crypto_onetimeauth.h
		src/libsodium/include/sodium/crypto_onetimeauth_poly1305.h
		src/libsodium/include/sodium/crypto_pwhash_scryptsalsa208sha256.h
		src/libsodium/include/sodium/crypto_scalarmult.h
		src/libsodium/include/sodium/crypto_scalarmult_curve25519.h
		src/libsodium/include/sodium/crypto_secretbox.h
		src/libsodium/include/sodium/crypto_secretbox_xsalsa20poly1305.h
		src/libsodium/include/sodium/crypto_shorthash.h
		src/libsodium/include/sodium/crypto_shorthash_siphash24.h
		src/libsodium/include/sodium/crypto_sign.h
		src/libsodium/include/sodium/crypto_sign_ed25519.h
		src/libsodium/include/sodium/crypto_sign_edwards25519sha512batch.h
		src/libsodium/include/sodium/crypto_stream.h
		src/libsodium/include/sodium/crypto_stream_aes128ctr.h
		src/libsodium/include/sodium/crypto_stream_chacha20.h
		src/libsodium/include/sodium/crypto_stream_salsa20.h
		src/libsodium/include/sodium/crypto_stream_salsa2012.h
		src/libsodium/include/sodium/crypto_stream_salsa208.h
		src/libsodium/include/sodium/crypto_stream_xsalsa20.h
		src/libsodium/include/sodium/crypto_uint16.h
		src/libsodium/include/sodium/crypto_uint32.h
		src/libsodium/include/sodium/crypto_uint64.h
		src/libsodium/include/sodium/crypto_uint8.h
		src/libsodium/include/sodium/crypto_verify_16.h
		src/libsodium/include/sodium/crypto_verify_32.h
		src/libsodium/include/sodium/crypto_verify_64.h
		src/libsodium/include/sodium/export.h
		src/libsodium/include/sodium/randombytes.h
		src/libsodium/include/sodium/randombytes_nativeclient.h
		src/libsodium/include/sodium/randombytes_salsa20_random.h
		src/libsodium/include/sodium/randombytes_sysrandom.h
		src/libsodium/include/sodium/runtime.h
		src/libsodium/include/sodium/utils.h
		)

set(LIBSODIUM_INCLUDES
		${ROOT_INCLUDES}
		${SODIUM_INCLUDES}
		)

set(LIBSODIUM_SRC
	src/libsodium/sodium/core.c
	src/libsodium/sodium/runtime.c
	src/libsodium/sodium/utils.c
	src/libsodium/sodium/version.c

	src/libsodium/randombytes/randombytes.c
	src/libsodium/randombytes/sysrandom/randombytes_sysrandom.c
	src/libsodium/randombytes/salsa20/randombytes_salsa20_random.c
	src/libsodium/randombytes/nativeclient/randombytes_nativeclient.c

	src/libsodium/crypto_verify/16/verify_16_api.c
	src/libsodium/crypto_verify/16/ref/verify_16.c
	src/libsodium/crypto_verify/32/verify_32_api.c
	src/libsodium/crypto_verify/32/ref/verify_32.c
	src/libsodium/crypto_verify/64/verify_64_api.c
	src/libsodium/crypto_verify/64/ref/verify_64.c

	src/libsodium/crypto_stream/crypto_stream.c
	src/libsodium/crypto_stream/xsalsa20/stream_xsalsa20_api.c
	src/libsodium/crypto_stream/xsalsa20/ref/stream_xsalsa20.c
	src/libsodium/crypto_stream/xsalsa20/ref/xor_xsalsa20.c
	src/libsodium/crypto_stream/salsa208/stream_salsa208_api.c
	src/libsodium/crypto_stream/salsa208/ref/stream_salsa208.c
	src/libsodium/crypto_stream/salsa208/ref/xor_salsa208.c
	src/libsodium/crypto_stream/salsa2012/stream_salsa2012_api.c
	src/libsodium/crypto_stream/salsa2012/ref/stream_salsa2012.c
	src/libsodium/crypto_stream/salsa2012/ref/stream_salsa2012.c
	src/libsodium/crypto_stream/salsa20/stream_salsa20_api.c
	src/libsodium/crypto_stream/salsa20/ref/stream_salsa20_ref.c
	src/libsodium/crypto_stream/salsa20/ref/xor_salsa20_ref.c
	src/libsodium/crypto_stream/salsa20/amd64_xmm6/stream_salsa20_amd64_xmm6.S #
	src/libsodium/crypto_stream/chacha20/stream_chacha20.c
	src/libsodium/crypto_stream/chacha20/ref/stream_chacha20_ref.c
	src/libsodium/crypto_stream/chacha20/vec/stream_chacha20_vec.c
	src/libsodium/crypto_stream/aes128ctr/stream_aes128ctr_api.c
	src/libsodium/crypto_stream/aes128ctr/portable/afternm_aes128ctr.c
	src/libsodium/crypto_stream/aes128ctr/portable/beforenm_aes128ctr.c
	src/libsodium/crypto_stream/aes128ctr/portable/common_aes128ctr.c
	src/libsodium/crypto_stream/aes128ctr/portable/consts_aes128ctr.c
	src/libsodium/crypto_stream/aes128ctr/portable/int128_aes128ctr.c
	src/libsodium/crypto_stream/aes128ctr/portable/stream_aes128ctr.c
	src/libsodium/crypto_stream/aes128ctr/portable/xor_afternm_aes128ctr.c

	src/libsodium/crypto_sign/crypto_sign.c
	src/libsodium/crypto_sign/ed25519/sign_ed25519_api.c
	src/libsodium/crypto_sign/ed25519/ref10/keypair.c
	src/libsodium/crypto_sign/ed25519/ref10/obsolete.c
	src/libsodium/crypto_sign/ed25519/ref10/open.c
	src/libsodium/crypto_sign/ed25519/ref10/sign.c

	src/libsodium/crypto_shorthash/crypto_shorthash.c
	src/libsodium/crypto_shorthash/siphash24/shorthash_siphash24_api.c
	src/libsodium/crypto_shorthash/siphash24/ref/shorthash_siphash24.c

	src/libsodium/crypto_secretbox/crypto_secretbox_easy.c
	src/libsodium/crypto_secretbox/crypto_secretbox.c
	src/libsodium/crypto_secretbox/xsalsa20poly1305/secretbox_xsalsa20poly1305_api.c
	src/libsodium/crypto_secretbox/xsalsa20poly1305/ref/box_xsalsa20poly1305.c

	src/libsodium/crypto_scalarmult/crypto_scalarmult.c
	src/libsodium/crypto_scalarmult/curve25519/scalarmult_curve25519.c
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/consts.S #
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/curve25519_sandy2x.c
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/fe_frombytes_sandy2x.c
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/fe51_invert.c
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/fe51_mul.S #
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/fe51_nsquare.S #
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/fe51_pack.S #
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/ladder.S #
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/ladder_base.S #
	src/libsodium/crypto_scalarmult/curve25519/sandy2x/sandy2x.S #
	src/libsodium/crypto_scalarmult/curve25519/ref10/x25519_ref10.c
	src/libsodium/crypto_scalarmult/curve25519/donna_c64/curve25519_donna_c64.c
	src/libsodium/crypto_scalarmult/curve25519/ref10/x25519_ref10.c

	src/libsodium/crypto_onetimeauth/crypto_onetimeauth.c
	src/libsodium/crypto_onetimeauth/poly1305/onetimeauth_poly1305.c
	src/libsodium/crypto_onetimeauth/poly1305/sse2/poly1305_sse2.c
	src/libsodium/crypto_onetimeauth/poly1305/donna/poly1305_donna.c

	src/libsodium/crypto_hash/crypto_hash.c
	src/libsodium/crypto_hash/sha512/hash_sha512_api.c
	src/libsodium/crypto_hash/sha512/cp/hash_sha512.c
	src/libsodium/crypto_hash/sha256/cp/hash_sha256.c
	src/libsodium/crypto_hash/sha256/hash_sha256_api.c

	src/libsodium/crypto_generichash/crypto_generichash.c
	src/libsodium/crypto_generichash/blake2/generichash_blake2_api.c
	src/libsodium/crypto_generichash/blake2/ref/blake2.h
	src/libsodium/crypto_generichash/blake2/ref/blake2b-compress-ref.c
	src/libsodium/crypto_generichash/blake2/ref/blake2b-compress-sse41.c
	src/libsodium/crypto_generichash/blake2/ref/blake2b-compress-ssse3.c
	src/libsodium/crypto_generichash/blake2/ref/blake2b-ref.c
	src/libsodium/crypto_generichash/blake2/ref/generichash_blake2b.c


	src/libsodium/crypto_core/curve25519/ref10/curve25519_ref10.c
	src/libsodium/crypto_core/hsalsa20/ref2/core_hsalsa20.c
	src/libsodium/crypto_core/hsalsa20/core_hsalsa20_api.c
	src/libsodium/crypto_core/salsa20/core_salsa20_api.c
	src/libsodium/crypto_core/salsa20/ref/core_salsa20.c
	src/libsodium/crypto_core/salsa2012/core_salsa2012_api.c
	src/libsodium/crypto_core/salsa2012/ref/core_salsa2012.c
	src/libsodium/crypto_core/salsa208/core_salsa208_api.c
	src/libsodium/crypto_core/salsa208/ref/core_salsa208.c

	src/libsodium/crypto_box/crypto_box.c
	src/libsodium/crypto_box/crypto_box_easy.c
	src/libsodium/crypto_box/crypto_box_seal.c
	src/libsodium/crypto_box/curve25519xsalsa20poly1305/box_curve25519xsalsa20poly1305_api.c
	src/libsodium/crypto_box/curve25519xsalsa20poly1305/ref/after_curve25519xsalsa20poly1305.c
	src/libsodium/crypto_box/curve25519xsalsa20poly1305/ref/before_curve25519xsalsa20poly1305.c
	src/libsodium/crypto_box/curve25519xsalsa20poly1305/ref/box_curve25519xsalsa20poly1305.c
	src/libsodium/crypto_box/curve25519xsalsa20poly1305/ref/keypair_curve25519xsalsa20poly1305.c

	src/libsodium/crypto_auth/crypto_auth.c
	src/libsodium/crypto_auth/hmacsha256/cp/hmac_hmacsha256.c
	src/libsodium/crypto_auth/hmacsha256/cp/verify_hmacsha256.c
	src/libsodium/crypto_auth/hmacsha256/auth_hmacsha256_api.c
	src/libsodium/crypto_auth/hmacsha512/cp/hmac_hmacsha512.c
	src/libsodium/crypto_auth/hmacsha512/cp/verify_hmacsha512.c
	src/libsodium/crypto_auth/hmacsha512/auth_hmacsha512_api.c
	src/libsodium/crypto_auth/hmacsha512256/auth_hmacsha512256_api.c
	src/libsodium/crypto_auth/hmacsha512256/cp/hmac_hmacsha512256.c
	src/libsodium/crypto_auth/hmacsha512256/cp/verify_hmacsha512256.c
	
	src/libsodium/crypto_aead/aes256gcm/aesni/aead_aes256gcm_aesni.c
	src/libsodium/crypto_aead/chacha20poly1305/sodium/aead_chacha20poly1305.c
	)

IF(LIBSODIUM_BUILD_SHARED_LIBRARIES)
    add_library(${PROJECT_NAME} SHARED
	${LIBSODIUM_INCLUDES}
	${LIBSODIUM_SRC})
ELSE(LIBSODIUM_BUILD_SHARED_LIBRARIES)
    add_definitions(-DSODIUM_STATIC)
    add_library(${PROJECT_NAME} STATIC
	${LIBSODIUM_INCLUDES}
	${LIBSODIUM_SRC})
ENDIF(LIBSODIUM_BUILD_SHARED_LIBRARIES)

# MSVC does not provide inline in C (only CXX)
IF(MSVC)
    add_definitions("-Dinline=__inline")
ENDIF(MSVC)

add_definitions(
	-DUNICODE
	-D_UNICODE)

install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION ${INSTALL_BIN_DIR}
		LIBRARY DESTINATION ${INSTALL_LIB_DIR}
		ARCHIVE DESTINATION ${INSTALL_ARCH_DIR})
		
install(FILES ${ROOT_INCLUDES}
		DESTINATION "${INSTALL_INC_ROOT_DIR}")
install(FILES ${SODIUM_INCLUDES}
		DESTINATION "${INSTALL_INC_SODIUM_DIR}")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/src/libsodium/include/sodium/version.h
		DESTINATION "${INSTALL_INC_SODIUM_DIR}")