# pvrdefs.mk -- symbols for use in building sgx files.

# Symbols which may be defined by this file (or not):
#   IS_SGX540=y
#   IS_SGX544=y
#   IS_SGX545=y

# Symbols that must be defined (or not) externally:
#   GFX_INCDIR
#   pvrdbg        may be defined as 1 or not.
#
# plus CONFIG symbols used:
#   CONFIG_DRM_CTP
#   CONFIG_DRM_CTP_PR1
#   CONFIG_DRM_MDFLD
#   CONFIG_DRM_MID_DEBUG
#   CONFIG_DRM_MID_RELEASE
#   CONFIG_MDFD_GL3
#   CONFIG_PCI_MSI

# Defined on the command line, but redundant, as would defined via .config
#   CONFIG_PCI_MSI
#   CONFIG_MDFD_GL3

# For what it's worth, the following defs from below are not found via:
#   grep -r -w symbol_name drivers/staging/mrst/pvr
# Some of these are probably defined because they may be used by the user
# mode sgx software.
#
#   CLOVERTRAIL_PHONE
#   MEDFIELD
#   OPK_FALLBACK
#   PROFILE_COMM
#   PVR2D_VALIDATE_INPUT_PARAMS
#   PVR_PROC_USE_SEQ_FILE
#   PVR_SECURE_FD_EXPORT
#   RELEASE
#   SERVICES4
#   SUPPORT_ANDROID_PLATFORM
#   SUPPORT_CPU_CACHED_BUFFERS
#   SUPPORT_DRI_DRM_NO_DROPMASTER
#   SUPPORT_LIBDRM_LITE
#   SUPPORT_SGX540
#   SUPPORT_SGX545
#   SUPPORT_SGX_EVENT_OBJECT
#   SUPPORT_SRVINIT
#   USE_PTHREADS
#   _XOPEN_SOURCE


PVRINCS-y := \
		-I$(GFX_INCDIR)/include4 \
		-I$(GFX_INCDIR)/services4/include \
		-I$(GFX_INCDIR)/services4/include/env/linux \
		-I$(GFX_INCDIR)/services4/srvkm/env/linux \
		-I$(GFX_INCDIR)/services4/srvkm/include \
		-I$(GFX_INCDIR)/services4/srvkm/bridged \
		-I$(GFX_INCDIR)/services4/system/include \
		-I$(GFX_INCDIR)/services4/srvkm/hwdefs \
		-I$(GFX_INCDIR)/services4/srvkm/bridged/sgx \
		-I$(GFX_INCDIR)/services4/srvkm/devices/sgx \
		-I$(GFX_INCDIR)/services4/srvkm/common \
		-I$(GFX_INCDIR)/services4/3rdparty/linux_framebuffer_drm \
		-I$(GFX_INCDIR)/services4/3rdparty/linux_framebuffer_mrst \
		-I$(GFX_INCDIR)/services4/system/intel_drm

PVRDEFS-y :=
PVRDEFS-y += -D_linux_ -D__KERNEL__

ifeq ($(pvrdbg),1)
PVRDEFS-y += -DPVRDEBUG
endif
ifeq ($(CONFIG_DRM_MDFLD),y)
PVRDEFS-y += -DMEDFIELD
IS_SGX540=y
endif
ifeq ($(CONFIG_DRM_CTP),y)
PVRDEFS-y += -DCLOVERTRAIL_PHONE
ifeq ($(CONFIG_DRM_CTP_PR1),y)
IS_SGX544=y
else
IS_SGX545=y
endif
endif

PVRDEFS-$(IS_SGX540) += -DSGX540 -DSUPPORT_SGX540 -DSGX_CORE_REV=121
PVRDEFS-$(IS_SGX544) += -DSGX544 -DSGX_CORE_REV=115 \
		-DSGX_FEATURE_MP=1 -DSGX_FEATURE_MP_CORE_COUNT=2 -DSGX_FEATURE_SYSTEM_CACHE=1 -DSUPPORT_MEMORY_TILING
PVRDEFS-$(IS_SGX545) += -DSGX545 -DSUPPORT_SGX545 -DSGX_CORE_REV=1014

PVRDEFS-y += \
		-DPVR_LINUX_TIMERS_USING_WORKQUEUES \
		-DLINUX \
		-DPVRSRV_MODNAME="\"pvrsrvkm\"" \
		-DPVR_BUILD_DIR="\"pc_i686_medfield_linux\"" \
		-DSERVICES4 \
		-D_XOPEN_SOURCE=600 \
		-DPVR2D_VALIDATE_INPUT_PARAMS \
		-DDISPLAY_CONTROLLER=mrstlfb \
		-UDEBUG_LOG_PATH_TRUNCATE \
		-DSUPPORT_LIBDRM_LITE \
		-DOPK_FALLBACK="" \
		-DPROFILE_COMM \
		-DPVR_LINUX_MISR_USING_PRIVATE_WORKQUEUE\
		-DSUPPORT_GRAPHICS_HAL \
		-DPVR_SECURE_FD_EXPORT \
		-DSUPPORT_SRVINIT \
		-DSUPPORT_SGX \
		-DSUPPORT_PERCONTEXT_PB \
		-DSUPPORT_LINUX_X86_WRITECOMBINE \
		-DTRANSFER_QUEUE \
		-DSUPPORT_DRI_DRM \
		-DSUPPORT_DRI_DRM_EXT \
		-DSUPPORT_DRI_DRM_NO_DROPMASTER \
		-DSYS_USING_INTERRUPTS \
		-DSUPPORT_HW_RECOVERY \
		-DSYS_SUPPORTS_SGX_IDLE_CALLBACK \
		-DSUPPORT_ACTIVE_POWER_MANAGEMENT \
		-DPVR_SECURE_HANDLES \
		-DLDM_PCI \
		-DUSE_PTHREADS \
		-DSUPPORT_SGX_EVENT_OBJECT \
		-DSUPPORT_SGX_HWPERF \
		-DSUPPORT_PVRSRV_GET_DC_SYSTEM_BUFFER \
		-DGBURST_HW_PVRSCOPESERVICE_SUPPORT \
		-DSUPPORT_LINUX_X86_PAT \
		-DPVR_PROC_USE_SEQ_FILE \
		-DSUPPORT_CPU_CACHED_BUFFERS \
		-DDRM_PVR_RESERVED_INTEL_ORDER \
		-DDRM_PVR_USE_INTEL_FB \
		-DSUPPORT_MEMINFO_IDS \
		-DSUPPORT_SGX_NEW_STATUS_VALS \
		-DSUPPORT_LARGE_GENERAL_HEAP \
		-DSUPPORT_DC_CMDCOMPLETE_WHEN_NO_LONGER_DISPLAYED\
		-DSUPPORT_CUSTOM_SWAP_OPERATIONS \
		-DPVRSRV_NEED_PVR_DPF \
		-DPVRSRV_NEW_PVR_DPF  \
		-DPVRSRV_NEED_PVR_ASSERT \
		-DPVR_LINUX_MEM_AREA_POOL_ALLOW_SHRINK \
		-DDRM_PVR_USE_INTEL_FB \
		-DPVRSRV_DUMP_MK_TRACE=1 \
		-DPVRSRV_USSE_EDM_STATUS_DEBUG=1 

#		-DSUPPORT_PDUMP_MULTI_PROCESS \


ifneq ($(IS_SGX545),y)
PVRDEFS-y += -DSUPPORT_SGX_LOW_LATENCY_SCHEDULING
endif

PVRDEFS-$(CONFIG_DRM_MID_RELEASE) += -DBUILD="\"release\"" -DPVR_BUILD_TYPE="\"release\"" -DRELEASE
PVRDEFS-$(CONFIG_DRM_MID_DEBUG) += -DBUILD="\"debug\"" -DPVR_BUILD_TYPE="\"debug\"" -DDEBUG -DDEBUG_LINUX_MEM_AREAS -DDEBUG_LINUX_MEMORY_ALLOCATIONS -DDEBUG_LINUX_MMAP_AREAS -DDEBUG_BRIDGE_KM
PVRDEFS-$(CONFIG_PCI_MSI) += -DCONFIG_PCI_MSI
PVRDEFS-$(CONFIG_MDFD_GL3) += -DSUPPORT_EXTERNAL_SYSTEM_CACHE -DCONFIG_MDFD_GL3
