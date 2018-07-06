/*
 * This file contains an array of opcode "simplifications".
 * It is generated from opcodes.list.
 */

#include "javavm/include/opcodes.h"

static const CVMOpcode CVMopcodeSimplification[256] = {
    opc_nop,    /* opc_nop */
    opc_iconst_0,    /* opc_aconst_null */
    opc_iconst_0,    /* opc_iconst_m1 */
    opc_iconst_0,    /* opc_iconst_0 */
    opc_iconst_0,    /* opc_iconst_1 */
    opc_iconst_0,    /* opc_iconst_2 */
    opc_iconst_0,    /* opc_iconst_3 */
    opc_iconst_0,    /* opc_iconst_4 */
    opc_iconst_0,    /* opc_iconst_5 */
    opc_iconst_0,    /* opc_lconst_0 */
    opc_iconst_0,    /* opc_lconst_1 */
    opc_iconst_0,    /* opc_fconst_0 */
    opc_iconst_0,    /* opc_fconst_1 */
    opc_iconst_0,    /* opc_fconst_2 */
    opc_iconst_0,    /* opc_dconst_0 */
    opc_iconst_0,    /* opc_dconst_1 */
    opc_iconst_0,    /* opc_bipush */
    opc_iconst_0,    /* opc_sipush */
    opc_ldc,    /* opc_ldc */
    opc_ldc,    /* opc_ldc_w */
    opc_ldc,    /* opc_ldc2_w */
    opc_iload,    /* opc_iload */
    opc_iload,    /* opc_lload */
    opc_iload,    /* opc_fload */
    opc_iload,    /* opc_dload */
    opc_aload,    /* opc_aload */
    opc_iload,    /* opc_iload_0 */
    opc_iload,    /* opc_iload_1 */
    opc_iload,    /* opc_iload_2 */
    opc_iload,    /* opc_iload_3 */
    opc_iload,    /* opc_lload_0 */
    opc_iload,    /* opc_lload_1 */
    opc_iload,    /* opc_lload_2 */
    opc_iload,    /* opc_lload_3 */
    opc_iload,    /* opc_fload_0 */
    opc_iload,    /* opc_fload_1 */
    opc_iload,    /* opc_fload_2 */
    opc_iload,    /* opc_fload_3 */
    opc_iload,    /* opc_dload_0 */
    opc_iload,    /* opc_dload_1 */
    opc_iload,    /* opc_dload_2 */
    opc_iload,    /* opc_dload_3 */
    opc_aload_0,    /* opc_aload_0 */
    opc_aload,    /* opc_aload_1 */
    opc_aload,    /* opc_aload_2 */
    opc_aload,    /* opc_aload_3 */
    opc_iaload,    /* opc_iaload */
    opc_iaload,    /* opc_laload */
    opc_iaload,    /* opc_faload */
    opc_iaload,    /* opc_daload */
    opc_iaload,    /* opc_aaload */
    opc_iaload,    /* opc_baload */
    opc_iaload,    /* opc_caload */
    opc_iaload,    /* opc_saload */
    opc_istore,    /* opc_istore */
    opc_istore,    /* opc_lstore */
    opc_istore,    /* opc_fstore */
    opc_istore,    /* opc_dstore */
    opc_istore,    /* opc_astore */
    opc_istore,    /* opc_istore_0 */
    opc_istore,    /* opc_istore_1 */
    opc_istore,    /* opc_istore_2 */
    opc_istore,    /* opc_istore_3 */
    opc_istore,    /* opc_lstore_0 */
    opc_istore,    /* opc_lstore_1 */
    opc_istore,    /* opc_lstore_2 */
    opc_istore,    /* opc_lstore_3 */
    opc_istore,    /* opc_fstore_0 */
    opc_istore,    /* opc_fstore_1 */
    opc_istore,    /* opc_fstore_2 */
    opc_istore,    /* opc_fstore_3 */
    opc_istore,    /* opc_dstore_0 */
    opc_istore,    /* opc_dstore_1 */
    opc_istore,    /* opc_dstore_2 */
    opc_istore,    /* opc_dstore_3 */
    opc_istore,    /* opc_astore_0 */
    opc_istore,    /* opc_astore_1 */
    opc_istore,    /* opc_astore_2 */
    opc_istore,    /* opc_astore_3 */
    opc_iastore,    /* opc_iastore */
    opc_iastore,    /* opc_lastore */
    opc_iastore,    /* opc_fastore */
    opc_iastore,    /* opc_dastore */
    opc_iastore,    /* opc_aastore */
    opc_iastore,    /* opc_bastore */
    opc_iastore,    /* opc_castore */
    opc_iastore,    /* opc_sastore */
    opc_pop,    /* opc_pop */
    opc_pop,    /* opc_pop2 */
    opc_dup,    /* opc_dup */
    opc_dup,    /* opc_dup_x1 */
    opc_dup,    /* opc_dup_x2 */
    opc_dup,    /* opc_dup2 */
    opc_dup,    /* opc_dup2_x1 */
    opc_dup,    /* opc_dup2_x2 */
    opc_dup,    /* opc_swap */
    opc_iadd,    /* opc_iadd */
    opc_iadd,    /* opc_ladd */
    opc_iadd,    /* opc_fadd */
    opc_iadd,    /* opc_dadd */
    opc_isub,    /* opc_isub */
    opc_isub,    /* opc_lsub */
    opc_isub,    /* opc_fsub */
    opc_isub,    /* opc_dsub */
    opc_imul,    /* opc_imul */
    opc_imul,    /* opc_lmul */
    opc_imul,    /* opc_fmul */
    opc_imul,    /* opc_dmul */
    opc_idiv,    /* opc_idiv */
    opc_idiv,    /* opc_ldiv */
    opc_idiv,    /* opc_fdiv */
    opc_idiv,    /* opc_ddiv */
    opc_irem,    /* opc_irem */
    opc_irem,    /* opc_lrem */
    opc_irem,    /* opc_frem */
    opc_irem,    /* opc_drem */
    opc_ineg,    /* opc_ineg */
    opc_ineg,    /* opc_lneg */
    opc_ineg,    /* opc_fneg */
    opc_ineg,    /* opc_dneg */
    opc_ishl,    /* opc_ishl */
    opc_ishl,    /* opc_lshl */
    opc_ishr,    /* opc_ishr */
    opc_ishr,    /* opc_lshr */
    opc_iushr,    /* opc_iushr */
    opc_iushr,    /* opc_lushr */
    opc_iand,    /* opc_iand */
    opc_iand,    /* opc_land */
    opc_ior,    /* opc_ior */
    opc_ior,    /* opc_lor */
    opc_ixor,    /* opc_ixor */
    opc_ixor,    /* opc_lxor */
    opc_iinc,    /* opc_iinc */
    opc_i2l,    /* opc_i2l */
    opc_i2l,    /* opc_i2f */
    opc_i2l,    /* opc_i2d */
    opc_i2l,    /* opc_l2i */
    opc_i2l,    /* opc_l2f */
    opc_i2l,    /* opc_l2d */
    opc_i2l,    /* opc_f2i */
    opc_i2l,    /* opc_f2l */
    opc_i2l,    /* opc_f2d */
    opc_i2l,    /* opc_d2i */
    opc_i2l,    /* opc_d2l */
    opc_i2l,    /* opc_d2f */
    opc_i2l,    /* opc_i2b */
    opc_i2l,    /* opc_i2c */
    opc_i2l,    /* opc_i2s */
    opc_lcmp,    /* opc_lcmp */
    opc_lcmp,    /* opc_fcmpl */
    opc_lcmp,    /* opc_fcmpg */
    opc_dcmpl,    /* opc_dcmpl */
    opc_dcmpg,    /* opc_dcmpg */
    opc_ifeq,    /* opc_ifeq */
    opc_ifeq,    /* opc_ifne */
    opc_ifeq,    /* opc_iflt */
    opc_ifeq,    /* opc_ifge */
    opc_ifeq,    /* opc_ifgt */
    opc_ifeq,    /* opc_ifle */
    opc_ifeq,    /* opc_if_icmpeq */
    opc_ifeq,    /* opc_if_icmpne */
    opc_ifeq,    /* opc_if_icmplt */
    opc_ifeq,    /* opc_if_icmpge */
    opc_ifeq,    /* opc_if_icmpgt */
    opc_ifeq,    /* opc_if_icmple */
    opc_ifeq,    /* opc_if_acmpeq */
    opc_ifeq,    /* opc_if_acmpne */
    opc_goto,    /* opc_goto */
    opc_jsr,    /* opc_jsr */
    opc_ret,    /* opc_ret */
    opc_tableswitch,    /* opc_tableswitch */
    opc_lookupswitch,    /* opc_lookupswitch */
    opc_return,    /* opc_ireturn */
    opc_return,    /* opc_lreturn */
    opc_return,    /* opc_freturn */
    opc_return,    /* opc_dreturn */
    opc_return,    /* opc_areturn */
    opc_return,    /* opc_return */
    opc_getstatic,    /* opc_getstatic */
    opc_putstatic,    /* opc_putstatic */
    opc_getfield,    /* opc_getfield */
    opc_putfield,    /* opc_putfield */
    opc_invokevirtual,    /* opc_invokevirtual */
    opc_invokespecial,    /* opc_invokespecial */
    opc_invokestatic,    /* opc_invokestatic */
    opc_invokeinterface,    /* opc_invokeinterface */
    opc_xxxunusedxxx,    /* opc_xxxunusedxxx */
    opc_new,    /* opc_new */
    opc_newarray,    /* opc_newarray */
    opc_anewarray,    /* opc_anewarray */
    opc_arraylength,    /* opc_arraylength */
    opc_athrow,    /* opc_athrow */
    opc_checkcast,    /* opc_checkcast */
    opc_instanceof,    /* opc_instanceof */
    opc_monitorenter,    /* opc_monitorenter */
    opc_monitorexit,    /* opc_monitorexit */
    opc_wide,    /* opc_wide */
    opc_multianewarray,    /* opc_multianewarray */
    opc_ifeq,    /* opc_ifnull */
    opc_ifeq,    /* opc_ifnonnull */
    opc_goto,    /* opc_goto_w */
    opc_jsr,    /* opc_jsr_w */
    opc_breakpoint,    /* opc_breakpoint */
    opc_ldc_quick,    /* opc_aldc_ind_quick */
    opc_ldc_quick,    /* opc_aldc_ind_w_quick */
    opc_invokestatic_quick,    /* opc_invokestatic_quick */
    opc_invokestatic_quick,    /* opc_invokestatic_checkinit_quick */
    opc_invokevirtual_quick,    /* opc_invokevirtual_quick */
    opc_getfield_quick,    /* opc_getfield_quick */
    opc_getfield_quick,    /* opc_agetfield_quick */
    opc_invokevirtual_quick,    /* opc_vinvokevirtual_quick */
    opc_invokevirtual_quick_w,    /* opc_invokevirtual_quick_w */
    opc_putfield_quick,    /* opc_putfield_quick */
    opc_invokenonvirtual_quick,    /* opc_invokenonvirtual_quick */
    opc_invokesuper_quick,    /* opc_invokesuper_quick */
    opc_invokeignored_quick,    /* opc_invokeignored_quick */
    opc_getfield_quick,    /* opc_getfield2_quick */
    opc_checkcast_quick,    /* opc_checkcast_quick */
    opc_instanceof_quick,    /* opc_instanceof_quick */
    opc_nonnull_quick,    /* opc_nonnull_quick */
    opc_putfield_quick,    /* opc_putfield2_quick */
    opc_invokevirtual_quick,    /* opc_ainvokevirtual_quick */
    opc_invokevirtualobject_quick,    /* opc_invokevirtualobject_quick */
    opc_invokeinterface_quick,    /* opc_invokeinterface_quick */
    opc_ldc_quick,    /* opc_aldc_quick */
    opc_ldc_quick,    /* opc_ldc_quick */
    opc_exittransition,    /* opc_exittransition */
    opc_invokevirtual_quick,    /* opc_dinvokevirtual_quick */
    opc_ldc_quick,    /* opc_aldc_w_quick */
    opc_ldc_quick,    /* opc_ldc_w_quick */
    opc_putfield_quick,    /* opc_aputfield_quick */
    opc_getfield_quick_w,    /* opc_getfield_quick_w */
    opc_ldc_quick,    /* opc_ldc2_w_quick */
    opc_getstatic_quick,    /* opc_agetstatic_quick */
    opc_getstatic_quick,    /* opc_getstatic_quick */
    opc_getstatic_quick,    /* opc_getstatic2_quick */
    opc_putstatic_quick,    /* opc_aputstatic_quick */
    opc_putstatic_quick,    /* opc_putstatic_quick */
    opc_putstatic_quick,    /* opc_putstatic2_quick */
    opc_getstatic_quick,    /* opc_agetstatic_checkinit_quick */
    opc_getstatic_quick,    /* opc_getstatic_checkinit_quick */
    opc_getstatic_quick,    /* opc_getstatic2_checkinit_quick */
    opc_putstatic_quick,    /* opc_aputstatic_checkinit_quick */
    opc_putstatic_quick,    /* opc_putstatic_checkinit_quick */
    opc_putstatic_quick,    /* opc_putstatic2_checkinit_quick */
    opc_putfield_quick_w,    /* opc_putfield_quick_w */
    opc_new_quick,    /* opc_new_checkinit_quick */
    opc_new_quick,    /* opc_new_quick */
    opc_anewarray_quick,    /* opc_anewarray_quick */
    opc_multianewarray_quick,    /* opc_multianewarray_quick */
    opc_prefix,    /* opc_prefix */
    opc_invokespecial,    /* opc_invokeinit */
    0,
    0,
    0,
    0,
};
