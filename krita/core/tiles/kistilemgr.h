/*
 *  Copyright (c) 2002 Patrick Julien <freak@codepimps.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#if !defined KISTILEMGR_H_
#define KISTILEMGR_H_

#include <qcstring.h>
#include <qglobal.h>
#include <qmutex.h>
#include <qpair.h>
#include <qvaluelist.h>
#include <ksharedptr.h>
#include "kistile.h"

class QPoint;
struct KisPixelData;
class KisTileMediator;

#define TILEMODE_NONE 0
#define TILEMODE_READ 1
#define TILEMODE_WRITE 2
#define TILEMODE_RW (TILEMODE_READ|TILEMODE_WRITE)

class KisTileMgr : public KShared {
	typedef KShared super;
	typedef QValueList<KisTileSP> KisTileSPLst;
	typedef KisTileSPLst::iterator KisTileSPLst_it;
	typedef KisTileSPLst::const_iterator KisTileSPLst_cit;
	typedef QValueVector<KisTileSPLst> vKisTileSPLst; 
	typedef vKisTileSPLst::iterator vKisTileSPLst_it; 
	typedef vKisTileSPLst::const_iterator vKisTileSPLst_cit; 

public:
	KisTileMgr(Q_UINT32 depth, Q_UINT32 width, Q_UINT32 height);
	KisTileMgr(KisTileMgr *tm, Q_UINT32 depth, Q_UINT32 width, Q_UINT32 height);
	KisTileMgr(const KisTileMgr& rhs);
	virtual ~KisTileMgr();

public:
	void attach(KisTileSP tile, Q_INT32 tilenum, bool keepold = true);
	void detach(KisTileSP tile, Q_INT32 tilenum, bool keepold = true);

	KisTileSP tile(Q_INT32 xpix, Q_INT32 ypix, Q_INT32 mode);
	KisTileSP tile(Q_INT32 tilenum, Q_INT32 mode);

	void tileMap(Q_INT32 xpix, Q_INT32 ypix, KisTileSP src);
	void tileMap(Q_INT32 tilenum, KisTileSP src);

	KisTileSP invalidate(Q_INT32 tileno);
	KisTileSP invalidate(Q_INT32 xpix, Q_INT32 ypix);
	KisTileSP invalidate(KisTileSP tile, Q_INT32 xpix, Q_INT32 ypix);
	void invalidateTiles(KisTileSP tile);

	bool empty() const;
	Q_INT32 width() const;
	Q_INT32 height() const;
	Q_UINT32 nrows() const;
	Q_UINT32 ncols() const;
	Q_INT32 depth() const;

	Q_UINT32 memSize();
	void tileCoord(const KisTileSP& tile, QPoint& coord);
	void tileCoord(const KisTileSP& tile, Q_INT32 *x, Q_INT32 *y);

	KisPixelDataSP pixelData(Q_INT32 x1, Q_INT32 y1, Q_INT32 x2, Q_INT32 y2, Q_INT32 mode);
	void releasePixelData(KisPixelDataSP pd);
	void readPixelData(Q_INT32 x1, Q_INT32 y1, Q_INT32 x2, Q_INT32 y2, QUANTUM *buffer, Q_UINT32 stride);
	void readPixelData(KisPixelDataSP pd);
	void writePixelData(Q_INT32 x1, Q_INT32 y1, Q_INT32 x2, Q_INT32 y2, QUANTUM *buffer, Q_UINT32 stride);
	void writePixelData(KisPixelDataSP pd);
	Q_INT32 tileNum(Q_UINT32 xpix, Q_UINT32 ypix) const;

private:
	KisTileMgr& operator=(const KisTileMgr&);
	void allocate(Q_INT32 ntiles);
	void duplicate(Q_INT32 ntiles, KisTileMgr *tm);
	KisTileSP invalidateTile(KisTileSP tile, Q_INT32 tilenum);

private:
	Q_UINT32 m_width;
	Q_UINT32 m_height;
	Q_UINT32 m_depth;
	Q_UINT32 m_ntileRows;
	Q_UINT32 m_ntileCols;
	vKisTileSP m_tiles;
	QMutex m_mutex;
	KisTileMediator *m_mediator;
};

#endif // KISTILEMGR_H_

