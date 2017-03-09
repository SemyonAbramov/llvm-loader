set(LLVM_TARGET_DEFINITIONS Attributes.td)
tablegen(LLVM Attributes.inc -gen-attrs)