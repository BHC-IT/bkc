cmake_minimum_required(VERSION 3.1)
project(LTC)

set(LTC_MAJOR_VERSION 0)
set(LTC_MINOR_VERSION 1)
set(LTC_PATCH_VERSION 0)
set(LTC_VERSION
	${LTC_MAJOR_VERSION}.${LTC_MINOR_VERSION}.${LTC_PATCH_VERSION}
	)


set (SRC
	src/hashes/md2.c
	src/hashes/md5.c
	src/hashes/sha1.c
	src/hashes/sha2/sha256.c
	src/hashes/sha2/sha512.c
	src/math/ltm_desc.c
	src/math/multi.c
	src/math/rand_prime.c
	src/misc/base64/base64_decode.c
	src/misc/base64/base64_encode.c
	src/misc/crypt/crypt_argchk.c
	src/misc/crypt/crypt_hash_descriptor.c
	src/misc/crypt/crypt_hash_is_valid.c
	src/misc/crypt/crypt_ltc_mp_descriptor.c
	src/misc/crypt/crypt_prng_descriptor.c
	src/misc/crypt/crypt_prng_is_valid.c
	src/misc/crypt/crypt_register_hash.c
	src/misc/crypt/crypt_register_prng.c
	src/misc/compare_testvector.c
	src/misc/error_to_string.c
	src/misc/mem_neq.c
	src/misc/pk_get_oid.c
	src/misc/zeromem.c
	src/pk/asn1/der/bit/der_decode_bit_string.c
	src/pk/asn1/der/bit/der_encode_bit_string.c
	src/pk/asn1/der/bit/der_decode_raw_bit_string.c
	src/pk/asn1/der/bit/der_encode_raw_bit_string.c
	src/pk/asn1/der/bit/der_length_bit_string.c
	src/pk/asn1/der/boolean/der_decode_boolean.c
	src/pk/asn1/der/boolean/der_encode_boolean.c
	src/pk/asn1/der/boolean/der_length_boolean.c
	src/pk/asn1/der/choice/der_decode_choice.c
	src/pk/asn1/der/generalizedtime/der_decode_generalizedtime.c
	src/pk/asn1/der/generalizedtime/der_encode_generalizedtime.c
	src/pk/asn1/der/generalizedtime/der_length_generalizedtime.c
	src/pk/asn1/der/ia5/der_decode_ia5_string.c
	src/pk/asn1/der/ia5/der_encode_ia5_string.c
	src/pk/asn1/der/ia5/der_length_ia5_string.c
	src/pk/asn1/der/integer/der_decode_integer.c
	src/pk/asn1/der/integer/der_encode_integer.c
	src/pk/asn1/der/integer/der_length_integer.c
	src/pk/asn1/der/object_identifier/der_decode_object_identifier.c
	src/pk/asn1/der/object_identifier/der_encode_object_identifier.c
	src/pk/asn1/der/object_identifier/der_length_object_identifier.c
	src/pk/asn1/der/octet/der_decode_octet_string.c
	src/pk/asn1/der/octet/der_encode_octet_string.c
	src/pk/asn1/der/octet/der_length_octet_string.c
	src/pk/asn1/der/printable_string/der_decode_printable_string.c
	src/pk/asn1/der/printable_string/der_encode_printable_string.c
	src/pk/asn1/der/printable_string/der_length_printable_string.c
	src/pk/asn1/der/sequence/der_decode_sequence_ex.c
	src/pk/asn1/der/sequence/der_decode_sequence_multi.c
	src/pk/asn1/der/sequence/der_decode_subject_public_key_info.c
	src/pk/asn1/der/sequence/der_encode_sequence_ex.c
	src/pk/asn1/der/sequence/der_encode_sequence_multi.c
	src/pk/asn1/der/sequence/der_encode_subject_public_key_info.c
	src/pk/asn1/der/sequence/der_length_sequence.c
	src/pk/asn1/der/set/der_encode_set.c
	src/pk/asn1/der/set/der_encode_setof.c
	src/pk/asn1/der/short_integer/der_decode_short_integer.c
	src/pk/asn1/der/short_integer/der_encode_short_integer.c
	src/pk/asn1/der/short_integer/der_length_short_integer.c
	src/pk/asn1/der/teletex_string/der_decode_teletex_string.c
	src/pk/asn1/der/teletex_string/der_length_teletex_string.c
	src/pk/asn1/der/utctime/der_decode_utctime.c
	src/pk/asn1/der/utctime/der_encode_utctime.c
	src/pk/asn1/der/utctime/der_length_utctime.c
	src/pk/asn1/der/utf8/der_decode_utf8_string.c
	src/pk/asn1/der/utf8/der_encode_utf8_string.c
	src/pk/asn1/der/utf8/der_length_utf8_string.c
	src/pk/ecc/ltc_ecc_map.c
	src/pk/ecc/ltc_ecc_mul2add.c
	src/pk/ecc/ltc_ecc_mulmod.c
	src/pk/ecc/ltc_ecc_mulmod_timing.c
	src/pk/ecc/ltc_ecc_points.c
	src/pk/ecc/ltc_ecc_projective_add_point.c
	src/pk/ecc/ltc_ecc_projective_dbl_point.c
	src/pk/pkcs1/pkcs_1_mgf1.c
	src/pk/pkcs1/pkcs_1_pss_decode.c
	src/pk/pkcs1/pkcs_1_pss_encode.c
	src/pk/pkcs1/pkcs_1_v1_5_decode.c
	src/pk/pkcs1/pkcs_1_v1_5_encode.c
	src/pk/rsa/rsa_export.c
	src/pk/rsa/rsa_exptmod.c
	src/pk/rsa/rsa_free.c
	src/pk/rsa/rsa_import.c
	src/pk/rsa/rsa_make_key.c
	src/pk/rsa/rsa_sign_hash.c
	src/pk/rsa/rsa_verify_hash.c
	src/prngs/rng_get_bytes.c
	src/prngs/sprng.c
	)

set (HEADER
	src/headers/tomcrypt_cfg.h
	src/headers/tomcrypt_mac.h
	src/headers/tomcrypt_macros.h
	src/headers/tomcrypt_custom.h
	src/headers/tomcrypt_argchk.h
	src/headers/tomcrypt_cipher.h
	src/headers/tomcrypt_pk.h
	src/headers/tomcrypt_hash.h
	src/headers/tomcrypt_math.h
	src/headers/tomcrypt_misc.h
	src/headers/tomcrypt.h
	src/headers/tomcrypt_pkcs.h
	src/headers/tomcrypt_prng.h
	src/hashes/sha2/sha224.c
	src/hashes/sha2/sha384.c
	)

set (GCC_COVERAGE_COMPILE_FLAGS "-O2 -fno-stack-protector -DNDEBUG -Wall -DLTC_SOURCE -DLTM_DESC -DMP_32BIT")

add_library(LTC STATIC ${SRC} ${HEADER})
target_include_directories(LTC PRIVATE src/headers)
target_include_directories(LTC PRIVATE ../libtommath/.)
target_include_directories(LTC PRIVATE .)

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
