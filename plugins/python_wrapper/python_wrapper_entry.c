/*
 * Remmina - The GTK+ Remote Desktop Client
 * Copyright (C) 2014-2021 Antenore Gatta, Giovanni Panozzo, Mathias Winterhalter (ToolsDevler)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 *  In addition, as a special exception, the copyright holders give
 *  permission to link the code of portions of this program with the
 *  OpenSSL library under certain conditions as described in each
 *  individual source file, and distribute linked combinations
 *  including the two.
 *  You must obey the GNU General Public License in all respects
 *  for all of the code used other than OpenSSL. *  If you modify
 *  file(s) with this exception, you may extend this exception to your
 *  version of the file(s), but you are not obligated to do so. *  If you
 *  do not wish to do so, delete this exception statement from your
 *  version. *  If you delete this exception statement from all source
 *  files in the program, then also delete it here.
 */

/**
 * @file python_wrapper_entry.c
 * @brief Contains the wiring of a Python pluing based on RemminaPluginProtocol.
 * @author Mathias Winterhalter
 * @date 07.04.2021
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "python_wrapper_common.h"
#include "python_wrapper_entry.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// D E C L A R A T I O N S
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A P I
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void python_wrapper_entry_init(void)
{
	TRACE_CALL(__func__);
}

void python_wrapper_entry_entry_func_wrapper(RemminaEntryPlugin* instance)
{
	TRACE_CALL(__func__);

	PyPlugin* plugin = python_wrapper_get_plugin(instance->name);
	if (plugin)
	{
		CallPythonMethod(plugin->instance, "entry_func", NULL);
	}
}

RemminaPlugin* python_wrapper_create_entry_plugin(PyPlugin* plugin)
{
	TRACE_CALL(__func__);

	PyObject* instance = plugin->instance;

	if (!python_wrapper_check_attribute(instance, ATTR_NAME)
		|| !python_wrapper_check_attribute(instance, ATTR_VERSION)
		|| !python_wrapper_check_attribute(instance, ATTR_DESCRIPTION))
	{
		g_printerr("Unable to create entry plugin. Aborting!\n");
		return NULL;
	}

	RemminaEntryPlugin* remmina_plugin = (RemminaEntryPlugin*)python_wrapper_malloc(sizeof(RemminaEntryPlugin));

	remmina_plugin->type = REMMINA_PLUGIN_TYPE_ENTRY;
	remmina_plugin->domain = GETTEXT_PACKAGE;
	remmina_plugin->name = PyUnicode_AsUTF8(PyObject_GetAttrString(instance, ATTR_NAME));
	remmina_plugin->version = PyUnicode_AsUTF8(PyObject_GetAttrString(instance, ATTR_VERSION));
	remmina_plugin->description = PyUnicode_AsUTF8(PyObject_GetAttrString(instance, ATTR_DESCRIPTION));
	remmina_plugin->entry_func = python_wrapper_entry_entry_func_wrapper;

	plugin->entry_plugin = remmina_plugin;
	plugin->generic_plugin = (RemminaPlugin*)remmina_plugin;

	python_wrapper_add_plugin(plugin);

	return (RemminaPlugin*)remmina_plugin;
}
