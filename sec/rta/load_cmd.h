#ifndef __RTA_LOAD_CMD_H__
#define __RTA_LOAD_CMD_H__

#define IMM_MUST 0
#define IMM_CAN  1
#define IMM_NO   2
#define IMM_DSNM 3 /* it doesn't matter the src type */

enum e_lenoff {
	LENOF_4,
	LENOF_48,
	LENOF_448,
	LENOF_18,
	LENOF_32,
	LENOF_24,
	LENOF_16,
	LENOF_8,
	LENOF_128,
	LENOF_64,
	DSNM /* it doesn't matter the lenght/offset values */
};

struct load_map {
	uint32_t dst;
	uint32_t dst_opcode;
	enum e_lenoff len_off;
	uint8_t imm_src;

};

static const struct load_map load_dst[36] = {
	{ _KEY1SZ,  LDST_CLASS_1_CCB | LDST_SRCDST_WORD_KEYSZ_REG,
		    LENOF_4,   IMM_MUST },
	{ _KEY2SZ,  LDST_CLASS_2_CCB | LDST_SRCDST_WORD_KEYSZ_REG,
		    LENOF_4,   IMM_MUST },
	{ _DATA1SZ, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_DATASZ_REG,
		    LENOF_448, IMM_MUST },
	{ _DATA2SZ, LDST_CLASS_2_CCB | LDST_SRCDST_WORD_DATASZ_REG,
		    LENOF_448, IMM_MUST },
	{ _ICV1SZ,  LDST_CLASS_1_CCB | LDST_SRCDST_WORD_ICVSZ_REG,
		    LENOF_4,   IMM_MUST },
	{ _ICV2SZ,  LDST_CLASS_2_CCB | LDST_SRCDST_WORD_ICVSZ_REG,
		    LENOF_4,   IMM_MUST },
	{ _DPID,    LDST_CLASS_DECO | LDST_SRCDST_WORD_PID,
		    LENOF_448, IMM_MUST },
	{ _CCTRL,   LDST_CLASS_IND_CCB | LDST_SRCDST_WORD_CHACTRL,
		    LENOF_4,   IMM_MUST },
	{ _DCTRL,   LDST_CLASS_DECO | LDST_IMM | LDST_SRCDST_WORD_DECOCTRL,
		    DSNM,      IMM_DSNM },
	{ _ICTRL,   LDST_CLASS_IND_CCB | LDST_SRCDST_WORD_IRQCTRL,
		    LENOF_4,   IMM_MUST },
	{ _DPOVRD,  LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_PCLOVRD,
		    LENOF_4,   IMM_MUST },
	{ _CLRW,    LDST_CLASS_IND_CCB | LDST_SRCDST_WORD_CLRW,
		    LENOF_4,   IMM_MUST },
	{ _AAD1SZ,  LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_AAD_SZ,
		    LENOF_4,   IMM_MUST },
	{ _IV1SZ,   LDST_CLASS_1_CCB | LDST_SRCDST_WORD_CLASS1_IV_SZ,
		    LENOF_4,   IMM_MUST },
	{ _ALTDS1,  LDST_CLASS_1_CCB | LDST_SRCDST_WORD_ALTDS_CLASS1,
		    LENOF_448, IMM_MUST },
	{ _PKASZ,   LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_A_SZ,
		    LENOF_4,   IMM_MUST, },
	{ _PKBSZ,   LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ,
		    LENOF_4,   IMM_MUST },
	{ _PKNSZ,   LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_N_SZ,
		    LENOF_4,   IMM_MUST },
	{ _PKESZ,   LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_E_SZ,
		    LENOF_4,   IMM_MUST },
	{ _IDFNS,   LDST_SRCDST_WORD_IFNSR, LENOF_18,  IMM_MUST },
	{ _ODFNS,   LDST_SRCDST_WORD_OFNSR, LENOF_18,  IMM_MUST },
	{ _ALTSOURCE, LDST_SRCDST_BYTE_ALTSOURCE,
		    LENOF_18,  IMM_MUST },
	{ _NFIFO,   LDST_CLASS_IND_CCB | LDST_SRCDST_WORD_INFO_FIFO,
		    LENOF_48,  IMM_MUST },
	{ _IFIFO,   LDST_SRCDST_BYTE_INFIFO,  LENOF_18, IMM_MUST },
	{ _OFIFO,   LDST_SRCDST_BYTE_OUTFIFO, LENOF_18, IMM_MUST },
	{ _MATH0,   LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH0,
		    LENOF_32,  IMM_CAN },
	{ _MATH1,   LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH1,
		    LENOF_24,  IMM_CAN },
	{ _MATH2,   LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH2,
		    LENOF_16,  IMM_CAN },
	{ _MATH3,   LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH3,
		    LENOF_8,   IMM_CAN },
	{ _CONTEXT1, LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT,
		    LENOF_128, IMM_CAN },
	{ _CONTEXT2, LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_CONTEXT,
		    LENOF_128, IMM_CAN },
	{ _KEY1,    LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_KEY,
		    LENOF_32,  IMM_CAN },
	{ _KEY2,    LDST_CLASS_2_CCB | LDST_SRCDST_BYTE_KEY,
		    LENOF_32,  IMM_CAN },
	{ _DESCBUF, LDST_CLASS_DECO | LDST_SRCDST_WORD_DESCBUF,
		    LENOF_64,  IMM_NO }
};

static inline int8_t load_check_len_offset(enum e_lenoff len_off, uint32_t length,
		uint32_t offset)
{
	switch (len_off) {
	case (LENOF_4):
		if ((length != 4) || (offset != 0))
			goto err;
		break;
	case (LENOF_48):
		if (!(((length == 4) && (offset == 0))
		      || ((length == 8) && (offset == 0))))
			goto err;
		break;
	case (LENOF_448):
		if (!(((length == 4) && (offset == 0))
		      || ((length == 4) && (offset == 4))
		      || ((length == 8) && (offset == 0))))
			goto err;
		break;
	case (LENOF_18):
		if ((length < 1) || (length > 8) || (offset != 0))
			goto err;
		break;
	case (LENOF_32):
		if ((length > 32) || (offset > 32) || ((offset + length) > 32))
			goto err;
		break;
	case (LENOF_24):
		if ((length > 24) || (offset > 24) || ((offset + length) > 24))
			goto err;
		break;
	case (LENOF_16):
		if ((length > 16) || (offset > 16) || ((offset + length) > 16))
			goto err;
		break;
	case (LENOF_8):
		if ((length > 8) || (offset > 8) || ((offset + length) > 8))
			goto err;
		break;
	case (LENOF_128):
		if ((length > 128) || (offset > 128) ||
				     ((offset + length) > 128))
			goto err;
		break;
	case (LENOF_64):
		if ((length < 1) || (length > 64) || ((length + offset) > 64))
			goto err;
		break;
	case (DSNM):
		break;
	default:
		goto err;
		break;
	}

	return 0;
err:
	return -1;
}

static inline uint32_t load(struct program *program, uint64_t src,
		uint32_t src_type, uint64_t dst, uint32_t dst_type,
		uint32_t offset, uint32_t length, uint32_t flags)
{
	uint32_t opcode = 0, cpy_length, i;
	uint8_t data_type = LDST_BYTE;
	uint8_t *tmp;
	uintptr_t src_ptr;
	int8_t pos = -1;

	if (flags & SEQ)
		opcode = CMD_SEQ_LOAD;
	else
		opcode = CMD_LOAD;

	if ((length & 0xffffff00) || (offset & 0xffffff00)) {
		pr_debug("Bad length/offset passed. Should be 8 bits\n");
		goto err;
	}

	if (flags & SGF)
		opcode |= LDST_SGF;
	if (flags & VLF)
		opcode |= LDST_VLF;

	/* check load destination, length and offset and source type */
	for (i = 0; i < sizeof(load_dst)/sizeof(load_dst[0]); i++)
		if (dst == load_dst[i].dst) {
			pos = i;
			break;
		}
	if (-1 == pos) {
		pr_debug("LOAD: Invalid dst. SEC Program Line: %d\n",
				program->current_pc);
		goto err;
	}

	if ((src_type == IMM_DATA) || (flags & IMMED)) {
		if (load_dst[pos].imm_src == IMM_NO) {
			pr_debug("LOAD: Invalid source type. "
					"SEC Program Line: %d\n",
					program->current_pc);
			goto err;
		}
		opcode |= LDST_IMM;
	} else if (load_dst[pos].imm_src == IMM_MUST) {
		pr_debug("LOAD IMM: Invalid source type. "
				"SEC Program Line: %d\n",
				program->current_pc);
		goto err;
	}

	if (-1 == load_check_len_offset(load_dst[pos].len_off, length,
					offset)) {
		pr_debug("LOAD: Invalid length/offset. SEC Program Line: %d\n",
				program->current_pc);
		goto err;
	}

	opcode |= load_dst[pos].dst_opcode;
	opcode |= length | (offset << LDST_OFFSET_SHIFT);

	program->buffer[program->current_pc++] = opcode;
	program->current_instraction++;

	/* DESC BUFFER: length/offset values are specified in 4-byte words */
	if (dst == _DESCBUF)
		data_type = LDST_WORD;

	/* DECO COTROL: skip writing pointer of imm data */
	if (dst == _DCTRL)
		return program->current_pc;

	/* Set size of data to be copied in descriptor */
	if (data_type == LDST_WORD)
		cpy_length = length * sizeof(uint32_t);
	else
		cpy_length = length;

	/* For data copy, 3 possible ways to specify how to copy data:
	 *  - src_type is IMM: copy data directly from src( max 8 bytes)
	 *  - src_type is PTR, but data should be immed: copy the data imm
	 *    from the location specified by user
	 *  - src_type is PTR and is not SEQ cmd: copy the address */
	if (src_type == IMM_DATA) {
		if (cpy_length <= BYTES_4) {
			program->buffer[program->current_pc++] = low_32b(src);
		} else {
			program->buffer[program->current_pc++] =
					high_32b(src);
			program->buffer[program->current_pc++] =
					low_32b(src);
			}
	}

	if ((src_type == PTR_DATA) && (flags & IMMED)) {
		src_ptr = (uintptr_t)src;
		tmp = (uint8_t *) &program->buffer[program->current_pc];

		for (i = 0; i < cpy_length; i++)
			*tmp++ = ((uint8_t *)src_ptr)[i];
		program->current_pc += ((cpy_length + 3) / 4);

		return program->current_pc;
	}

	if ((src_type == PTR_DATA) && (!(flags & SEQ))) {
		if (program->ps == 1) {
				program->buffer[program->current_pc++] =
					high_32b(src);
				program->buffer[program->current_pc++] =
					low_32b(src);
			}
		else {
			program->buffer[program->current_pc++] =
				low_32b(src);
		}
	}

	return program->current_pc;
 err:
	program->first_error_pc = program->current_pc;
	program->current_instraction++;
	return program->current_pc++;
}

#endif /* __RTA_LOAD_CMD_H__*/