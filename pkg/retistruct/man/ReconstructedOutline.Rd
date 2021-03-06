% Generated by roxygen2 (4.1.0): do not edit by hand
% Please edit documentation in R/ReconstructedOutline.R
\name{ReconstructedOutline}
\alias{ReconstructedOutline}
\title{Reconstruct outline into spherical surface}
\usage{
ReconstructedOutline(o, n = 500, alpha = 8, x0 = 0.5, report = print,
  plot.3d = FALSE, dev.flat = NA, dev.polar = NA)
}
\arguments{
\item{o}{\code{\link{AnnotatedOutline}} object, containing the following information:\describe{
\item{\code{P}}{outline points as N-by-2 matrix}
\item{\code{V0}}{indicies of the apex of each tear}
\item{\code{VF}}{indicies of the forward vertex of each tear}
\item{\code{VB}}{indicies of the backward vertex of each tear}
\item{\code{i0}}{index of the landmark on the rim}
\item{\code{phi0}}{lattitude of rim of partial sphere}
\item{\code{lambda0}}{longitude of landmark on rim}
}}

\item{n}{Number of points in triangulation.}

\item{alpha}{Area scaling coefficient}

\item{x0}{Area cutoff coefficient}

\item{report}{Function used to report progress.}

\item{plot.3d}{Whether to show 3D picture during optimisation.}

\item{dev.flat}{Device to plot grid onto. Value of \code{NA} (default)
means no plotting.}

\item{dev.polar}{Device display projection. Value of NA
(default) means no plotting.}
}
\value{
\code{reconstructedOutline} object containing the input
information and the following modified and extra information:
\item{\code{P}}{New set of points in flattened object}
\item{\code{gf}}{New set of forward pointers in flattened object}
\item{\code{gb}}{New set of backward pointers in flattened object}
\item{\code{phi}}{lattitude of new points on sphere}
\item{\code{lambda}}{longitude of new points on sphere}
\item{\code{Tt}}{New triangulation}
}
\description{
Reconstruct outline into spherical surface. Reconstruction
proceeds in a number of stages:
}
\details{
\enumerate{

\item The flat object is triangulated with at least \code{n}
triangles. This can introduce new vertices in the rim.

\item The triangulated object is stitched.

\item The stitched object is triangulated again, but this time it
is not permitted to add extra vertices to the rim.

\item The corresponding points determined by the stitching process
are merged to form a new set of merged points and a new
triangulation.

\item The merged points are projected roughly to a sphere.

\item The locations of the points on the sphere are moved so as to
minimise the energy function.
}
}
\author{
David Sterratt
}

