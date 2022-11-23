# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
"""TVM Runtime NDArray API.

tvm.ndarray provides a minimum runtime array API to test
the correctness of the program.
"""
# pylint: disable=invalid-name,unused-import

from __future__ import absolute_import as _abs
import numpy as _np
from user_struct import *
import struct
from ._ffi.ndarray import DEContext, DEType, NDArrayBase, _make_array
from ._ffi.ndarray import context, empty, empty_dp, from_dlpack, _ext_info
from ._ffi.ndarray import _set_class_ndarray
from ._ffi.ndarray import register_extension, free_extension_handle
from ._ffi.ndarray import DENDArrayExtInfo, DENDArrayExtInfoHandle, Rect
from ._ffi.function import *
from ._ffi.function import Function

class NDArray(NDArrayBase):
    """Lightweight NDArray class of TVM runtime.

    Strictly this is only an Array Container (a buffer object)
    No arthimetic operations are defined.
    All operations are performed by TVM functions.

    The goal is not to re-build yet another array library.
    Instead, this is a minimal data structure to demonstrate
    how can we use TVM in existing project which might have their own array containers.
    """
    
def ext_info(format=0, shape_indicator=0, roi_rect = Rect(0,0,0,0)):
    """Construct a addition info

    Parameters
    ----------
    format : int32, optional
        The Img format
    shape_indicator:
        Indicator Img layout (width height cell) in shape
        0 :hwc 1: whc
    Returns
    -------
    ctx : TVMNDArrayAdditionInfo
        The created addition info
    """
    return _ext_info(format, shape_indicator, roi_rect)

def cpu(dev_id=0):
    """Construct a CPU device

    Parameters
    ----------
    dev_id : int, optional
        The integer device id

    Returns
    -------
    ctx : DEContext
        The created context
    """
    return DEContext(1, dev_id)


def gpu(dev_id=0):
    """Construct a CPU device

    Parameters
    ----------
    dev_id : int, optional
        The integer device id

    Returns
    -------
    ctx : DEContext
        The created context
    """
    return DEContext(2, dev_id)

def rocm(dev_id=0):
    """Construct a ROCM device

    Parameters
    ----------
    dev_id : int, optional
        The integer device id

    Returns
    -------
    ctx : DEContext
        The created context
    """
    return DEContext(10, dev_id)


def opencl(dev_id=0):
    """Construct a OpenCL device

    Parameters
    ----------
    dev_id : int, optional
        The integer device id

    Returns
    -------
    ctx : DEContext
        The created context
    """
    return DEContext(4, dev_id)


def metal(dev_id=0):
    """Construct a metal device

    Parameters
    ----------
    dev_id : int, optional
        The integer device id

    Returns
    -------
    ctx : DEContext
        The created context
    """
    return DEContext(8, dev_id)


def vpi(dev_id=0):
    """Construct a VPI simulated device

    Parameters
    ----------
    dev_id : int, optional
        The integer device id

    Returns
    -------
    ctx : DEContext
        The created context
    """
    return DEContext(9, dev_id)


def vulkan(dev_id=0):
    """Construct a Vulkan device

    Parameters
    ----------
    dev_id : int, optional
        The integer device id

    Returns
    -------
    ctx : DEContext
        The created context
    """
    return DEContext(7, dev_id)


def opengl(dev_id=0):
    """Construct a OpenGL device

    Parameters
    ----------
    dev_id : int, optional
        The integer device id

    Returns
    -------
    ctx : DEContext
        The created context
    """
    return DEContext(11, dev_id)


def ext_dev(dev_id=0):
    """Construct a extension device

    Parameters
    ----------
    dev_id : int, optional
        The integer device id

    Returns
    -------
    ctx : DEContext
        The created context

    Note
    ----
    This API is reserved for quick testing of new
    device by plugin device API as ext_dev.
    """
    return DEContext(12, dev_id)


cl = opencl
mtl = metal


def array(arr, ctx=cpu(0)):
    """Create an array from source arr.

    Parameters
    ----------
    arr : numpy.ndarray
        The array to be copied from

    ctx : DEContext, optional
        The device context to create the array

    Returns
    -------
    ret : NDArray
        The created array
    """
    if not isinstance(arr, (_np.ndarray, NDArray)):
        arr = _np.array(arr)
    return empty(arr.shape, arr.dtype, ctx).copyfrom(arr)

def array_dp(arr, ctx=cpu(0), info=ext_info(0, 0, Rect(0,0,0,0))):
    """Create an array from source arr.

    Parameters
    ----------
    arr : numpy.ndarray
        The array to be copied from

    ctx : DEContext, optional
        The device context to create the array

    additional_info : additional_info, optional
        The ndarray additional info

    Returns
    -------
    ret : NDArray
        The created array
    """
    if not isinstance(arr, (_np.ndarray, NDArray)):
        arr = _np.array(arr)
    return empty_dp(arr.shape, arr.dtype, ctx, info).copyfrom(arr)

def depackage(vec_array_bytes):
    """ depackage 由dmlc::toByteArray序列化后的vector<ndarray>码流 

     Parameters
    ----------
    vec_array_bytes : 序列化后的vector<ndarray>码流

     Returns
    -------
    list_array : ndarray list

    """
    vec_array_str = get_global_func("vec.ndarray.depackage")(vec_array_bytes)
    vec_array = VecInt64.from_bytes(vec_array_str)
    list_array = []
    for idx in range(vec_array.num):
        list_array.append(_make_array(vec_array.ptr[idx], False, True))

    return list_array

_set_class_ndarray(NDArray)
