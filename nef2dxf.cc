/*
 *  OpenSCAD (www.openscad.at)
 *  Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#define INCLUDE_ABSTRACT_NODE_DETAILS

#include "openscad.h"

DxfData::DxfData(const struct CGAL_Nef_polyhedron &N)
{
	Grid2d<int> grid;

	typedef CGAL_Nef_polyhedron2::Explorer Explorer;
	typedef Explorer::Face_const_iterator fci_t;
	typedef Explorer::Halfedge_around_face_const_circulator heafcc_t;
	Explorer E = N.p2.explorer();

	for (fci_t fit = E.faces_begin(), fend = E.faces_end(); fit != fend; ++fit)
	{
		heafcc_t fcirc(E.halfedge(fit)), fend(fcirc);
		int first_point = -1;
		CGAL_For_all(fcirc, fend) {
			if (E.is_standard(E.target(fcirc))) {
				Explorer::Point ep = E.point(E.target(fcirc));
				double x = to_double(ep.x()), y = to_double(ep.y());
				int this_point = -1;
				if (grid.has(x, y)) {
					this_point = grid.align(x, y);
				} else {
					this_point = grid.align(x, y) = points.size();
					points.append(Point(x, y));
				}
				if (first_point < 0) {
					paths.append(Path());
					first_point = this_point;
				}
				paths.last().points.append(&points[this_point]);
			}
		}
		if (first_point >= 0) {
			paths.last().is_closed = 1;
			paths.last().points.append(&points[first_point]);
		}
	}
}
