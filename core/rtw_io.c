/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 ******************************************************************************/
/*

The purpose of rtw_io.c

a. provides the API

b. provides the protocol engine

c. provides the software interface between caller and the hardware interface

Compiler Flag Option:

1. For USB:
   a. USE_ASYNC_IRP: Both sync/async operations are provided.

Only sync read/rtw_write_mem operations are provided.

jackson@realtek.com.tw

*/

#define _RTW_IO_C_
#include <osdep_service.h>
#include <drv_types.h>
#include <rtw_io.h>
#include <osdep_intf.h>

#include <usb_ops.h>

u8 _rtw_read823a(struct rtw_adapter *adapter, u32 addr)
{
	u8 r_val;
	struct intf_hdl *pintfhdl = &adapter->intf;

	r_val = pintfhdl->io_ops._read8(pintfhdl, addr);

	return r_val;
}

u16 _rtw_read1623a(struct rtw_adapter *adapter, u32 addr)
{
	u16 r_val;
	struct intf_hdl *pintfhdl = &adapter->intf;

	r_val = pintfhdl->io_ops._read16(pintfhdl, addr);

	return le16_to_cpu(r_val);
}

u32 _rtw_read3223a(struct rtw_adapter *adapter, u32 addr)
{
	u32 r_val;
	struct intf_hdl *pintfhdl = &adapter->intf;

	r_val = pintfhdl->io_ops._read32(pintfhdl, addr);

	return le32_to_cpu(r_val);
}

int _rtw_write823a(struct rtw_adapter *adapter, u32 addr, u8 val)
{
	struct intf_hdl *pintfhdl = &adapter->intf;
	int ret;

	ret = pintfhdl->io_ops._write8(pintfhdl, addr, val);

	return RTW_STATUS_CODE23a(ret);
}

int _rtw_write1623a(struct rtw_adapter *adapter, u32 addr, u16 val)
{
	struct intf_hdl *pintfhdl = &adapter->intf;
	int ret;

	val = cpu_to_le16(val);
	ret = pintfhdl->io_ops._write16(pintfhdl, addr, val);

	return RTW_STATUS_CODE23a(ret);
}

int _rtw_write3223a(struct rtw_adapter *adapter, u32 addr, u32 val)
{
	struct intf_hdl *pintfhdl = &adapter->intf;
	int ret;

	val = cpu_to_le32(val);
	ret = pintfhdl->io_ops._write32(pintfhdl, addr, val);

	return RTW_STATUS_CODE23a(ret);
}

int _rtw_writeN23a(struct rtw_adapter *adapter, u32 addr , u32 length , u8 *pdata)
{
        struct intf_hdl *pintfhdl = &adapter->intf;
	int ret;

	ret = pintfhdl->io_ops._writeN(pintfhdl, addr, length, pdata);

	return RTW_STATUS_CODE23a(ret);
}
void _rtw_read_mem23a(struct rtw_adapter *adapter, u32 addr, u32 cnt, u8 *pmem)
{
	struct intf_hdl *pintfhdl = &adapter->intf;

	if ((adapter->bDriverStopped == true) ||
	    (adapter->bSurpriseRemoved == true)) {
	     RT_TRACE(_module_rtl871x_io_c_, _drv_info_,
		      ("rtw_read_mem:bDriverStopped(%d) OR "
		       "bSurpriseRemoved(%d)", adapter->bDriverStopped,
		       adapter->bSurpriseRemoved));
	     return;
	}

	pintfhdl->io_ops._read_mem(pintfhdl, addr, cnt, pmem);
}

void _rtw_write_mem23a(struct rtw_adapter *adapter, u32 addr, u32 cnt, u8 *pmem)
{
	struct intf_hdl *pintfhdl = &adapter->intf;

	pintfhdl->io_ops._write_mem(pintfhdl, addr, cnt, pmem);
}

void _rtw_read_port23a(struct rtw_adapter *adapter, u32 addr, u32 cnt,
		    struct recv_buf *rbuf)
{
	struct intf_hdl *pintfhdl = &adapter->intf;

	if ((adapter->bDriverStopped == true) ||
	    (adapter->bSurpriseRemoved == true)) {
	     RT_TRACE(_module_rtl871x_io_c_, _drv_info_,
		      ("rtw_read_port:bDriverStopped(%d) OR "
		       "bSurpriseRemoved(%d)", adapter->bDriverStopped,
		       adapter->bSurpriseRemoved));
	     return;
	}

	pintfhdl->io_ops._read_port(pintfhdl, addr, cnt, rbuf);
}

void _rtw_read_port23a_cancel(struct rtw_adapter *adapter)
{
	void (*_read_port_cancel)(struct intf_hdl *pintfhdl);
	struct intf_hdl *pintfhdl = &adapter->intf;

	_read_port_cancel = pintfhdl->io_ops._read_port_cancel;

	if (_read_port_cancel)
		_read_port_cancel(pintfhdl);
}

u32 _rtw_write_port23a(struct rtw_adapter *adapter, u32 addr, u32 cnt,
		    struct xmit_buf *xbuf)
{
	struct intf_hdl *pintfhdl = &adapter->intf;
	u32 ret = _SUCCESS;

	ret = pintfhdl->io_ops._write_port(pintfhdl, addr, cnt, xbuf);

	return ret;
}

void _rtw_write_port23a_cancel(struct rtw_adapter *adapter)
{
	void (*_write_port_cancel)(struct intf_hdl *pintfhdl);
	struct intf_hdl *pintfhdl = &adapter->intf;

	_write_port_cancel = pintfhdl->io_ops._write_port_cancel;

	if (_write_port_cancel)
		_write_port_cancel(pintfhdl);
}
