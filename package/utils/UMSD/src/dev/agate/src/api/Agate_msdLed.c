#include <msdCopyright.h>

/*******************************************************************************
* Agate_msdLed.h
*
* DESCRIPTION:
*       API/Structure definitions for Marvell LED functionality.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include <agate/include/api/Agate_msdLed.h>
#include <agate/include/api/Agate_msdApiInternal.h>
#include <agate/include/driver/Agate_msdHwAccess.h>
#include <agate/include/driver/Agate_msdDrvSwRegs.h>
#include <msdSem.h>
#include <msdHwAccess.h>
#include <msdUtils.h>


/****************************************************************************/
/* internal LED related function declaration.                                    */
/****************************************************************************/
static MSD_STATUS Agate_LedCtrl_Write
(
IN	MSD_QD_DEV	*dev,
IN  MSD_LPORT  portNum,
IN	MSD_U8	pointer,
IN	MSD_U16	data
);
static MSD_STATUS Agate_LedCtrl_Read
(
IN	MSD_QD_DEV	*dev,
IN  MSD_LPORT  portNum,
IN	MSD_U8	pointer,
OUT	MSD_U16	*data
);

/*******************************************************************************
* Agate_gledForceOn
*
* DESCRIPTION:
*       This routine is to Force Port LED On
*
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       ledNum  - LED Index
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Agate_gledForceOn
(
IN	MSD_QD_DEV	*dev,
IN  MSD_LPORT portNum,
IN  MSD_U8    ledNum
)
{
    MSD_STATUS retVal;
    MSD_U8 pointer;
    MSD_U8 foff;
    MSD_U8 flen;
    MSD_U16 val;
    MSD_U16 tmp;

    MSD_DBG_INFO(("Agate_gledForceOn Called.\n"));

    if (portNum > 5)
    {
        MSD_DBG_ERROR(("Bad Port: %u. It should be within [0,5]\n", (unsigned int)portNum));
        return MSD_BAD_PARAM;
    }

    switch (ledNum)
    {
        case 0:
            pointer = 0;
            foff = 0;
            flen = 4;
            val = 0xf;
            break;
        case 1:
            pointer = 0;
            foff = 4;
            flen = 4;
            val = 0xf;
            break;
        default:
            MSD_DBG_ERROR(("Bad LED: %u. It should be within [0,1]\n", (unsigned int)ledNum));
            return MSD_BAD_PARAM;    
    }

    retVal = Agate_LedCtrl_Read(dev, portNum, pointer, &tmp);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Agate_LedCtrl_Read returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_BF_SET(tmp, val, foff, flen);

    retVal = Agate_LedCtrl_Write(dev, portNum, pointer, tmp);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Agate_LedCtrl_Write returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Agate_gledForceOn Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Agate_gledForceOff
*
* DESCRIPTION:
*       This routine is to Force Port LED Off
*
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       ledNum  - LED Index
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Agate_gledForceOff
(
IN	MSD_QD_DEV	*dev,
IN  MSD_LPORT portNum,
IN  MSD_U8    ledNum
)
{
    MSD_STATUS retVal;
    MSD_U8 pointer;
    MSD_U8 foff;
    MSD_U8 flen;
    MSD_U16 val;
    MSD_U16 tmp;

    MSD_DBG_INFO(("Agate_gledForceOff Called.\n"));

    if(portNum > 5)
    {
        MSD_DBG_ERROR(("Bad Port: %u. It should be within [0,5]\n", (unsigned int)portNum));
        return MSD_BAD_PARAM;
    }

    switch (ledNum)
    {
        case 0:
            pointer = 0;
            foff = 0;
            flen = 4;
            val = 0xe;
            break;
        case 1:
            pointer = 0;
            foff = 4;
            flen = 4;
            val = 0xe;
            break;
        default:
            MSD_DBG_ERROR(("Bad LED: %u. It should be within [0,1]\n", (unsigned int)ledNum));
            return MSD_BAD_PARAM;
    }

    retVal = Agate_LedCtrl_Read(dev, portNum, pointer, &tmp);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Agate_LedCtrl_Read returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_BF_SET(tmp, val, foff, flen);

    retVal = Agate_LedCtrl_Write(dev, portNum, pointer, tmp);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Agate_LedCtrl_Write returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Agate_gledForceOff Exit.\n"));
    return MSD_OK;
}

/*******************************************************************************
* Agate_gledLinkAct
*
* DESCRIPTION:
*       This routine is to set Port LED mode to link/act. That is:
*           off - no link, on - link, blink = activity.
*
*
* INPUTS:
*       devNum  - physical devie number
*       portNum - logical port number
*       ledNum  - LED Index
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL  - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*       MSD_NOT_SUPPORTED - device not support
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Agate_gledLinkAct
(
IN	MSD_QD_DEV	*dev,
IN  MSD_LPORT portNum,
IN  MSD_U8    ledNum
)
{
    MSD_STATUS retVal;
    MSD_U8 pointer;
    MSD_U8 foff;
    MSD_U8 flen;
    MSD_U16 val;
    MSD_U16 tmp;

    MSD_DBG_INFO(("Agate_gledLinkAct Called.\n"));

    if (portNum > 5)
    {
        MSD_DBG_ERROR(("Bad Port: %u. It should be within [0,5]\n", (unsigned int)portNum));
        return MSD_BAD_PARAM;
    }

    switch (ledNum)
    {
        case 0:
            pointer = 0;
            foff = 0;
            flen = 4;
			if (portNum >= 0 || portNum <= 4)
			{
				val = 0x3;
			}
			else
			{
				val = 0x7;
			}
            break;
        case 1:
            pointer = 0;
            foff = 4;
            flen = 4;
			if (portNum >= 0 || portNum <= 4)
			{
				MSD_DBG_ERROR(("LED: %u not support LinkAct mode\n", (unsigned int)ledNum));
				return MSD_NOT_SUPPORTED;
			}
			else
			{
				val = 0x4;
			}
            break;
        default:
            MSD_DBG_ERROR(("Bad LED: %u. It should be within [0,1]\n", (unsigned int)ledNum));
            return MSD_BAD_PARAM;
    }

    retVal = Agate_LedCtrl_Read(dev, portNum, pointer, &tmp);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Agate_LedCtrl_Read returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_BF_SET(tmp, val, foff, flen);

    retVal = Agate_LedCtrl_Write(dev, portNum, pointer, tmp);
    if (retVal != MSD_OK)
    {
        MSD_DBG_ERROR(("Agate_LedCtrl_Write returned: %s.\n", msdDisplayStatus(retVal)));
        return retVal;
    }

    MSD_DBG_INFO(("Agate_gledLinkAct Exit.\n"));
    return MSD_OK;
}

/****************************************************************************/
/* Internal functions.                                                  */
/****************************************************************************/
static MSD_STATUS Agate_LedCtrl_Write
(
IN	MSD_QD_DEV	*dev,
IN  MSD_LPORT  portNum,
IN	MSD_U8	pointer,
IN	MSD_U16	data
)
{
    MSD_STATUS    retVal;         /* Functions return value.      */
    MSD_U8        hwPort;         /* the physical port number     */
    MSD_U8		 phyAddr;
    MSD_U16   count, tmpData;

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(portNum);
    phyAddr = AGATE_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        return MSD_BAD_PARAM;
    }

    msdSemTake(dev->devNum, dev->tblRegsSem, OS_WAIT_FOREVER);

    count = 10;
    tmpData = 1;
    while (tmpData == 1)
    {
        retVal = msdGetAnyRegField(dev->devNum, phyAddr, AGATE_QD_REG_LED_CONTROL, 15, 1, &tmpData);
        if (retVal != MSD_OK)
        {
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return retVal;
        }
        if ((count--) == 0)
        {
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return MSD_FAIL;
        }
    }

    tmpData = (MSD_U16)((1 << 15) | ((pointer & 0x7) << 12) | (data & 0x7FF));

    retVal = msdSetAnyReg(dev->devNum, phyAddr, AGATE_QD_REG_LED_CONTROL, tmpData);
    if (retVal != MSD_OK)
    {
        msdSemGive(dev->devNum, dev->tblRegsSem);
        return retVal;
    }

    msdSemGive(dev->devNum, dev->tblRegsSem);
    return retVal;
}

static MSD_STATUS Agate_LedCtrl_Read
(
IN	MSD_QD_DEV	*dev,
IN  MSD_LPORT  portNum,
IN	MSD_U8	pointer,
OUT	MSD_U16	*data
)
{
    MSD_STATUS    retVal;         /* Functions return value.      */
    MSD_U8           hwPort;         /* the physical port number     */
    MSD_U8			phyAddr;
    MSD_U16            tmpData;
    int count = 0x10;

    /* translate LPORT to hardware port */
    hwPort = MSD_LPORT_2_PORT(portNum);
    phyAddr = AGATE_MSD_CALC_SMI_DEV_ADDR(dev, hwPort);
    if (hwPort == MSD_INVALID_PORT)
    {
        return MSD_BAD_PARAM;
    }

    msdSemTake(dev->devNum, dev->tblRegsSem, OS_WAIT_FOREVER);

    do {
        retVal = msdGetAnyReg(dev->devNum, phyAddr, AGATE_QD_REG_LED_CONTROL, &tmpData);
        if (retVal != MSD_OK)
        {
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return retVal;
        }
        if ((count--) == 0)
        {
            msdSemGive(dev->devNum, dev->tblRegsSem);
            return MSD_FAIL;
        }
    } while (tmpData & 0x8000);

    tmpData = (MSD_U16)(((pointer & 0x7) << 12) | 0);
    retVal = msdSetAnyReg(dev->devNum, phyAddr, AGATE_QD_REG_LED_CONTROL, tmpData);
    if (retVal != MSD_OK)
    {
        msdSemGive(dev->devNum, dev->tblRegsSem);
        return retVal;
    }

    retVal = msdGetAnyReg(dev->devNum, phyAddr, AGATE_QD_REG_LED_CONTROL, &tmpData);
    if (retVal != MSD_OK)
    {
        msdSemGive(dev->devNum, dev->tblRegsSem);
        return retVal;
    }

    *data = (MSD_U8)(tmpData & 0x7ff);
    msdSemGive(dev->devNum, dev->tblRegsSem);

    return retVal;
}