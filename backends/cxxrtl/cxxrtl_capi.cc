/*
 *  yosys -- Yosys Open SYnthesis Suite
 *
 *  Copyright (C) 2020  whitequark <whitequark@whitequark.org>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

// This file is a part of the CXXRTL C API. It should be used together with `cxxrtl_capi.h`.

#include <backends/cxxrtl/cxxrtl.h>
#include <backends/cxxrtl/cxxrtl_capi.h>

struct _cxxrtl_handle {
	std::unique_ptr<cxxrtl::module> module;
	cxxrtl::debug_items objects;
};

cxxrtl_handle cxxrtl_create(cxxrtl_toplevel design) {
	cxxrtl_handle handle = new _cxxrtl_handle;
	handle->module = std::move(design->module);
	handle->module->debug_info(handle->objects);
	delete design;
	return handle;
}

void cxxrtl_destroy(cxxrtl_handle handle) {
	delete handle;
}

size_t cxxrtl_step(cxxrtl_handle handle) {
	return handle->module->step();
}

cxxrtl_object *cxxrtl_get(cxxrtl_handle handle, const char *name) {
	if (handle->objects.count(name) > 0)
		return static_cast<cxxrtl_object*>(&handle->objects.at(name));
	return nullptr;
}

void cxxrtl_enum(cxxrtl_handle handle, void *data,
                 void (*callback)(void *data, const char *name, struct cxxrtl_object *object)) {
	for (auto &it : handle->objects)
		callback(data, it.first.c_str(), static_cast<cxxrtl_object*>(&it.second));
}
