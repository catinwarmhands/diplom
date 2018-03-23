// POST

int main() {
	auto stack_data = (char*)malloc(STACK_SIZE);
	auto stack_pointer = stack_data + STACK_SIZE;

	global_context = (__s0_Context_instance0*)malloc(sizeof(__s0_Context_instance0));
	global_context->m_thread_index = 0xABCD;
	global_context->m_user_index = 0;
	global_context->m_user_data = nullptr;
	global_context->m_allocator = nullptr;
	global_context->m_allocator_data = nullptr;
	global_context->m_logger = nullptr;
	global_context->m_logger_data = nullptr;

	for(auto i = 0; i < sizeof(rodata_self_ref)/sizeof(SELF_REF) - 1; i++) {
		auto self = &rodata_self_ref[i];
		auto ptr = rodata + self->offset;
		memcpy(rodata + self->index, &ptr, sizeof(void*));
	}
	
	// PUSH CONTEXT
	_context_init0_1r1_v_pt7Context(global_context);

    // CREATE TEMP CONTEXT
    auto context = (__s0_Context_instance0*)malloc(sizeof(__s0_Context_instance0));
    memcpy(context, global_context, sizeof(__s0_Context_instance0));

	// UPDATE GLOBALS
	_global_init0_0r1_v(context);
	_global_init_save0_0r1_v(context);

	_main0_0r1_v(context);
	return 0;
}


/*
int main() {
	auto stack_data = (char*)malloc(STACK_SIZE);
	auto stack_pointer = stack_data + STACK_SIZE;

	global_context = (__s0_Context*)malloc(sizeof(__s0_Context));
	global_context->m_thread_index = 0xABCD;
	global_context->m_user_index = 0;
	global_context->m_user_data = nullptr;
	global_context->m_allocator = nullptr;
	global_context->m_allocator_data = nullptr;
	global_context->m_logger = nullptr;
	global_context->m_logger_data = nullptr;


	for(auto i = 0; i < sizeof(rodata_self_ref)/sizeof(SELF_REF) - 1; i++) {
		auto self = &rodata_self_ref[i];
		auto ptr = rodata + self->offset;
		memcpy(rodata + self->index, &ptr, sizeof(void*));
	}
	
	_reg r14{};
	r14.ptr = stack_pointer;
	(r14).u64 -= sizeof(size_t);
    *((__Context**)((r14).ptr)) = global_context;

	// PUSH CONTEXT
	(r14).u64 -= sizeof(size_t);
    stack_pointer = (char*)(r14.ptr);
	_context_init0_1r1_v_pt7Context(stack_pointer);

	// UPDATE GLOBALS
	_global_init0_0r1_v(stack_pointer);
	_global_init_save0_0r1_v(stack_pointer);

	// CREATE TEMP CONTEXT
	auto context = (__s0_Context*)malloc(sizeof(__s0_Context));
	memcpy(context, global_context, sizeof(__s0_Context));


	// PUSH CONTEXT
	r14.ptr = stack_pointer;
	(r14).u64 -= sizeof(size_t);
    *((__Context**)((r14).ptr)) = context;
    
	(r14).u64 -= sizeof(size_t);
    stack_pointer = (char*)(r14.ptr);
	_reg r = _main0_0r1_v(stack_pointer);

	return r.s32;
}
*/