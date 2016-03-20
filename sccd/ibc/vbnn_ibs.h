#pragma once

#include <sccd/core/ec.h>
#include <sccd/core/fp.h>

typedef struct {
	sccd_fp_t private_key;
	sccd_ec_t public_key;
} sccd_vbnn_ibs_ta_t;

typedef struct {
	sccd_ec_t R;
	sccd_fp_t s;
} sccd_vbnn_ibs_user_t;

typedef struct {
	sccd_ec_t R;
	sccd_fp_t h;
	sccd_fp_t z;
} sccd_vbnn_ibs_sig_t;

void sccd_vbnn_ibs_ta_init(const sccd_fp_t random, sccd_vbnn_ibs_ta_t* ta);
void sccd_vbnn_ibs_ta_extract_key(const sccd_fp_t random, const sccd_vbnn_ibs_ta_t* ta, const uint8_t* id, const size_t id_len, sccd_vbnn_ibs_user_t *extracted_user_key);
void sccd_vbnn_ibs_sign(const sccd_fp_t random, const sccd_vbnn_ibs_user_t* user_key, const uint8_t* id, const size_t id_len, const uint8_t* msg, const size_t msg_len, sccd_vbnn_ibs_sig_t* signature);
int sccd_vbnn_ibs_verify(const sccd_ec_t ta_public, const uint8_t* id, const size_t id_len, const uint8_t* msg, const size_t msg_len, const sccd_vbnn_ibs_sig_t* signature);
