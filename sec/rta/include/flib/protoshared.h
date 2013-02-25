#ifndef __RTA_PROTOSHARED_H__
#define __RTA_PROTOSHARED_H__

#include "rta.h"

/**
 * @file                 protoshared.h
 * @brief                CAAM Descriptor Construction Library Protocol-level
 *                       Shared Descriptor Constructors
 */

/**
 * @defgroup sharedesc_group Shared Descriptor Example Routines
 * @{
 */

/**
 * @details              SNOW/f8 (UEA2) as a shared descriptor.
 *
 * @param [in] descbuf   Pointer to descriptor-under-construction buffer.
 * @param [in] bufsize   Points to size to be updated at completion.
 * @param [in] cipherkey Cipher key.
 * @param [in] keylen    Size of key in bits.
 * @param [in] dir       Cipher direction (DIR_ENCRYPT/DIR_DECRYPT).
 * @param [in] count     UEA2 count value (32 bits).
 * @param [in] bearer    UEA2 bearer ID (5 bits).
 * @param [in] direction UEA2 direction (1 bit).
 *
 */
static inline void cnstr_shdsc_snow_f8(uint32_t *descbuf, uint16_t *bufsize,
			 uint8_t *cipherkey, uint32_t keylen,
			 uint8_t dir, uint32_t count,
			 uint8_t bearer, uint8_t direction)
{
	struct program prg;
	struct program *program = &prg;
	uint64_t ct = count;
	uint64_t br = bearer;
	uint64_t dr = direction;
	uint64_t context = (ct << 32) | (br << 27) | (dr << 26);

	PROGRAM_CNTXT_INIT(descbuf, 0);
	SHR_HDR(SHR_ALWAYS, 1, 0);
	{
		KEY(KEY1, 0, PTR((uintptr_t)cipherkey), keylen, IMMED);
		MATHB(SEQINSZ, SUB, MATH2, VSEQINSZ, SIZE(4), 0);
		MATHB(SEQINSZ, SUB, MATH2, VSEQOUTSZ, SIZE(4), 0);
		ALG_OPERATION(OP_ALG_ALGSEL_SNOW_F8, OP_ALG_AAI_F8,
			      OP_ALG_AS_INITFINAL, 0, dir);
		LOAD(IMM(context), CONTEXT1, 0, SIZE(8), 0);
		SEQFIFOLOAD(MSG1, SIZE(32), WITH(VLF | LAST1 | LAST2));
		SEQFIFOSTORE(MSG, 0, SIZE(32), WITH(VLF));
	}
	*bufsize = PROGRAM_FINALIZE();
}

/**
 * @details             SNOW/f9 (UIA2) as a shared descriptor.
 *
 * @param[in] descbuf   Pointer to descriptor-under-construction buffer.
 * @param[in] bufsize   Points to size to be updated at completion.
 * @param[in] cipherkey Cipher key.
 * @param[in] keylen    Size of key.
 * @param[in] dir       Cipher direction (DIR_ENCRYPT/DIR_DECRYPT).
 * @param[in] count     UEA2 count value (32 bits).
 * @param[in] fresh     UEA2 fresh value ID (32 bits).
 * @param[in] direction UEA2 direction (1 bit).
 * @param[in] datalen   Size of data.
 */
static inline void cnstr_shdsc_snow_f9(uint32_t *descbuf, uint16_t *bufsize,
			 uint8_t *cipherkey, uint32_t keylen,
			 uint8_t dir, uint32_t count,
			 uint32_t fresh, uint8_t direction,
			 uint32_t datalen)
{
	struct program prg;
	struct program *program = &prg;
	uint64_t ct = count;
	uint64_t fr = fresh;
	uint64_t dr = direction;
	uint64_t context[2];

	context[0] = (ct << 32) | (dr << 26);
	context[1] = fr << 32;

	PROGRAM_CNTXT_INIT(descbuf, 0);
	SHR_HDR(SHR_ALWAYS, 1, 0);
	{
		KEY(KEY2, 0, PTR((uintptr_t)cipherkey), keylen, IMMED);
		MATHB(SEQINSZ, SUB, MATH2, VSEQINSZ, SIZE(4), 0);
		ALG_OPERATION(OP_ALG_ALGSEL_SNOW_F9, OP_ALG_AAI_F9,
			      OP_ALG_AS_INITFINAL, 0, dir);
		LOAD(PTR((uintptr_t)context), CONTEXT2, 0, SIZE(16), IMMED);
		SEQFIFOLOAD(BIT_DATA, datalen, WITH(CLASS2 | LAST1 | LAST2));
		/* Save lower half of MAC out into a 32-bit sequence */
		SEQSTORE(CONTEXT2, 0, SIZE(4), 0);
	}
	*bufsize = PROGRAM_FINALIZE();
}

/**
 * @details             CBC blockcipher
 *
 * @param[in] descbuf   Pointer to descriptor-under-construction buffer.
 * @param[in] bufsize   Limit/returned descriptor buffer size.
 * @param[in] cipherkey Key data to inline.
 * @param[in] keylen    Key length.
 * @param[in] iv        IV data.
 * @param[in] ivlen     IV length.
 * @param[in] dir       DIR_ENCRYPT/DIR_DECRYPT.
 * @param[in] cipher    OP_ALG_ALGSEL_AES/DES/3DES.
 */
static inline void cnstr_shdsc_cbc_blkcipher(uint32_t *descbuf, uint16_t *bufsize,
			       uint8_t *cipherkey, uint32_t keylen,
			       uint8_t *iv, uint32_t ivlen,
			       uint8_t dir, uint32_t cipher)
{
	struct program prg;
	struct program *program = &prg;

	PROGRAM_CNTXT_INIT(descbuf, 0);
	SHR_HDR(SHR_ALWAYS, 1, SC);
	{
		/* Insert Key */
		KEY(KEY1, 0, PTR((uintptr_t)cipherkey), keylen, IMMED);
		MATHB(SEQINSZ, SUB, MATH2, VSEQINSZ, SIZE(4), 0);
		MATHB(SEQINSZ, SUB, MATH2, VSEQOUTSZ, SIZE(4), 0);
		ALG_OPERATION(cipher, OP_ALG_AAI_CBC, OP_ALG_AS_INIT, 0, dir);
		/* IV load, convert size */
		LOAD(PTR((uintptr_t)iv), CONTEXT1, 0, ivlen, IMMED);
		/* Insert sequence load/store with VLF */
		SEQFIFOLOAD(MSG1, SIZE(32), WITH(VLF | LAST1 | LAST2));
		SEQFIFOSTORE(MSG, 0, SIZE(32), WITH(VLF));
	}
	*bufsize = PROGRAM_FINALIZE();
}

/**
 * @details            HMAC shared
 *
 * @param[in] descbuf  Pointer to descriptor-under-construction buffer.
 * @param[in] bufsize  Limit/returned descriptor buffer size.
 * @param[in] md_key   Key data to inline (length based on message digest
 *                     algorithm).
 * @param[in] md_algo  Message digest algorithm: OP_ALG_ALGSEL_MD5/SHA1-512.
 * @param[in] icv      HMAC comparison for ICV, NULL if no check desired.
 */
static inline void cnstr_shdsc_hmac(uint32_t *descbuf, uint16_t *bufsize,
		      uint8_t *md_key, uint32_t md_algo, uint8_t *icv)
{
	struct program prg;
	struct program *program = &prg;
	uint8_t storelen;
	uint8_t opicv;

	/* Compute fixed-size store based on alg selection */
	switch (md_algo) {
	case OP_ALG_ALGSEL_MD5:
		storelen = 16;
		break;
	case OP_ALG_ALGSEL_SHA1:
		storelen = 20;
		break;
	case OP_ALG_ALGSEL_SHA224:
		storelen = 28;
		break;
	case OP_ALG_ALGSEL_SHA256:
		storelen = 32;
		break;
	case OP_ALG_ALGSEL_SHA384:
		storelen = 48;
		break;
	case OP_ALG_ALGSEL_SHA512:
		storelen = 64;
		break;
	default:
		return;
	}

	opicv = icv ? ICV_CHECK_ENABLE : ICV_CHECK_DISABLE;

	PROGRAM_CNTXT_INIT(descbuf, 0);
	SHR_HDR(SHR_ALWAYS, 1, SC);
	{
		KEY(KEY2, 0, PTR((uintptr_t)md_key), storelen, IMMED);
		/* compute sequences */
		MATHB(SEQINSZ, SUB, MATH2, VSEQINSZ, SIZE(4), 0);
		MATHB(SEQINSZ, SUB, MATH2, VSEQOUTSZ, SIZE(4), 0);
		/* Do operation */
		ALG_OPERATION(md_algo, OP_ALG_AAI_HMAC, OP_ALG_AS_INITFINAL,
			      opicv, DIR_ENC);
		/* Do load (variable length) */
		SEQFIFOLOAD(MSG2, SIZE(32), WITH(VLF | LAST1 | LAST2));
		SEQSTORE(CONTEXT2, 0, storelen, 0);
	}
	*bufsize = PROGRAM_FINALIZE();
}

/**
 * @details            KASUMI F8 (Confidentialy) as a shared descriptor (ETSI
 *                     "Document 1: f8 and f9 specification").
 *
 * @param[in] descbuf   Pointer to descriptor-under-construction buffer.
 * @param[in] bufsize   Points to size to be updated at completion.
 * @param[in] cipherkey Cipher key.
 * @param[in] keylen    Size of key.
 * @param[in] dir       Cipher direction (DIR_ENCRYPT/DIR_DECRYPT).
 * @param[in] count     Count value (32 bits).
 * @param[in] bearer    Bearer ID (5 bits).
 * @param[in] direction Direction (1 bit).
 */
static inline void cnstr_shdsc_kasumi_f8(uint32_t *descbuf, uint16_t *bufsize,
			   uint8_t *cipherkey, uint32_t keylen,
			   uint8_t dir, uint32_t count,
			   uint8_t bearer, uint8_t direction)
{
	struct program prg;
	struct program *program = &prg;
	uint64_t ct = count;
	uint64_t br = bearer;
	uint64_t dr = direction;
	uint64_t context = (ct << 32) | (br << 27) | (dr << 26);

	PROGRAM_CNTXT_INIT(descbuf, 0);
	SHR_HDR(SHR_ALWAYS, 1, 0);
	{
		KEY(KEY1, 0, PTR((uintptr_t)cipherkey), keylen, IMMED);
		MATHB(SEQINSZ, SUB, MATH2, VSEQINSZ, SIZE(4), 0);
		MATHB(SEQINSZ, SUB, MATH2, VSEQOUTSZ, SIZE(4), 0);
		ALG_OPERATION(OP_ALG_ALGSEL_KASUMI, OP_ALG_AAI_F8,
			      OP_ALG_AS_INITFINAL, 0, dir);
		LOAD(IMM(context), CONTEXT1, 0, SIZE(8), 0);
		SEQFIFOLOAD(MSG1, SIZE(32), WITH(VLF | LAST1 | LAST2));
		SEQFIFOSTORE(MSG, 0, SIZE(32), WITH(VLF));
	}
	*bufsize = PROGRAM_FINALIZE();
}

/**
 * @details             KASUMI F9 (Integrity) as a shared descriptor (ETSI
 *                      "Document 1: f8 and f9 specification").
 *
 * @param[in] descbuf   Pointer to descriptor-under-construction buffer.
 * @param[in] bufsize   Points to size to be updated at completion.
 * @param[in] cipherkey Cipher key.
 * @param[in] keylen    Size of key.
 * @param[in] dir       Cipher direction (DIR_ENCRYPT/DIR_DECRYPT).
 * @param[in] count     Count value (32 bits).
 * @param[in] fresh     Fresh value ID (32 bits).
 * @param[in] direction Direction (1 bit).
 * @param[in] datalen   Size of data.
 */
static inline void cnstr_shdsc_kasumi_f9(uint32_t *descbuf, uint16_t *bufsize,
			   uint8_t *cipherkey, uint32_t keylen,
			   uint8_t dir, uint32_t count,
			   uint32_t fresh, uint8_t direction,
			   uint32_t datalen)
{
	struct program prg;
	struct program *program = &prg;
	uint16_t ctx_offset = 16;
	uint64_t ct = count;
	uint64_t fr = fresh;
	uint64_t dr = direction;
	uint64_t context[3];

	context[0] = (ct << 32) | (dr << 26);
	context[1] = (fr << 32);

	PROGRAM_CNTXT_INIT(descbuf, 0);
	SHR_HDR(SHR_ALWAYS, 1, 0);
	{
		KEY(KEY1, 0, PTR((uintptr_t)cipherkey), keylen, IMMED);
		MATHB(SEQINSZ, SUB, MATH2, VSEQINSZ, SIZE(4), 0);
		ALG_OPERATION(OP_ALG_ALGSEL_KASUMI, OP_ALG_AAI_F9,
			      OP_ALG_AS_INITFINAL, 0, dir);
		LOAD(PTR((uintptr_t)context), CONTEXT1, 0, SIZE(24), IMMED);
		SEQFIFOLOAD(BIT_DATA, datalen, WITH(CLASS1 | LAST1 | LAST2));
		/* Save output MAC of DWORD 2 into a 32-bit sequence */
		SEQSTORE(CONTEXT1, ctx_offset, SIZE(4), 0);
	}
	*bufsize = PROGRAM_FINALIZE();
}

/**
 * @details            CRC32 Accelerator (IEEE 802 CRC32 protocol mode)
 *
 * @param[in] descbuf  Pointer to descriptor-under-construction buffer.
 * @param[in] bufsize  Limit of descriptor buffer size.
 */
static inline void cnstr_shdsc_crc(uint32_t *descbuf, uint16_t *bufsize)
{
	struct program prg;
	struct program *program = &prg;

	PROGRAM_CNTXT_INIT(descbuf, 0);
	SHR_HDR(SHR_ALWAYS, 1, 0);
	{
		MATHB(SEQINSZ, SUB, MATH2, VSEQINSZ, SIZE(4), 0);
		ALG_OPERATION(OP_ALG_ALGSEL_CRC,
			      OP_ALG_AAI_802 | OP_ALG_AAI_DOC,
			      OP_ALG_AS_FINALIZE, 0, DIR_ENC);
		SEQFIFOLOAD(MSG2, SIZE(32), WITH(VLF | LAST2));
		SEQSTORE(CONTEXT2, 0, SIZE(4), 0);
	}
	*bufsize = PROGRAM_FINALIZE();
}

/**
 * @details		MACsec(802.1AE) encapsulation
 *
 * @param[in] descbuf   Pointer to descriptor-under-construction buffer.
 * @param[in] bufsize   Points to size to be updated at completion.
 * @param[in] cipherkey Key data to inline.
 * @param[in] keylen    Key size in bytes.
 * @param[in] sci       PDB Secure Channel Identifier.
 * @param[in] ethertype PDB EtherType.
 * @param[in] tci_an    TAG Control Information and Association Number
 *                      are treated as a single field of 8 bits in PDB.
 * @param[in] pn        PDB Packet Number.
 **/
static inline void cnstr_shdsc_macsec_encap(uint32_t *descbuf,
					    uint16_t *bufsize,
					    uint8_t *cipherkey,
					    uint32_t keylen, uint64_t sci,
					    uint16_t ethertype, uint8_t tci_an,
					    uint32_t pn)
{
	struct program prg;
	struct program *program = &prg;
	struct macsec_encap_pdb pdb;
	uint32_t startidx;

	LABEL(keyjump);
	REFERENCE(pkeyjump);

	memset(&pdb, 0x00, sizeof(struct macsec_encap_pdb));
	pdb.sci_hi = high_32b(sci);
	pdb.sci_lo = low_32b(sci);
	pdb.ethertype = ethertype;
	pdb.tci_an = tci_an;
	pdb.pn = pn;

	startidx = sizeof(struct macsec_encap_pdb) >> 2;

	PROGRAM_CNTXT_INIT(descbuf, 0);
	SHR_HDR(SHR_SERIAL, ++startidx, WITH(SC));
	{
		ENDIAN_DATA((uint8_t *)&pdb, sizeof(struct macsec_encap_pdb));
		pkeyjump = program->current_pc;
		JUMP(IMM(keyjump), LOCAL_JUMP, ALL_TRUE,
		     WITH(SHRD | SELF | BOTH));
		KEY(KEY1, 0, PTR((uintptr_t) cipherkey), keylen, WITH(IMMED));
		SET_LABEL(keyjump);
		PROTOCOL(OP_TYPE_ENCAP_PROTOCOL, OP_PCLID_MACSEC,
			 WITH(OP_PCL_MACSEC));
	}
	PATCH_JUMP(program, pkeyjump, keyjump);
	*bufsize = PROGRAM_FINALIZE();
}

/**
 * @details		MACsec(802.1AE) decapsulation
 *
 * @param[in] descbuf   Pointer to descriptor-under-construction buffer.
 * @param[in] bufsize   Points to size to be updated at completion.
 * @param[in] cipherkey Key data to inline.
 * @param[in] keylen    Key size in bytes.
 * @param[in] sci       PDB Secure Channel Identifier.
 * @param[in] pn        PDB Packet Number.
 **/
static inline void cnstr_shdsc_macsec_decap(uint32_t *descbuf,
					    uint16_t *bufsize,
					    uint8_t *cipherkey,
					    uint32_t keylen, uint64_t sci,
					    uint32_t pn)
{
	struct program prg;
	struct program *program = &prg;
	struct macsec_decap_pdb pdb;
	uint32_t startidx;

	LABEL(keyjump);
	REFERENCE(pkeyjump);

	memset(&pdb, 0x00, sizeof(struct macsec_decap_pdb));
	pdb.sci_hi = high_32b(sci);
	pdb.sci_lo = low_32b(sci);
	pdb.pn = pn;

	startidx = sizeof(struct macsec_decap_pdb) >> 2;

	PROGRAM_CNTXT_INIT(descbuf, 0);
	SHR_HDR(SHR_SERIAL, ++startidx, WITH(SC));
	{
		ENDIAN_DATA((uint8_t *)&pdb, sizeof(struct macsec_decap_pdb));
		pkeyjump = program->current_pc;
		JUMP(IMM(keyjump), LOCAL_JUMP, ALL_TRUE,
		     WITH(SHRD | SELF | BOTH));
		KEY(KEY1, 0, PTR((uintptr_t) cipherkey), keylen, WITH(IMMED));
		SET_LABEL(keyjump);
		PROTOCOL(OP_TYPE_DECAP_PROTOCOL, OP_PCLID_MACSEC,
			 WITH(OP_PCL_MACSEC));
	}
	PATCH_JUMP(program, pkeyjump, keyjump);
	*bufsize = PROGRAM_FINALIZE();
}

/** @} */ /* end of sharedesc_group */

#endif /* __RTA_PROTOSHARED_H__ */
