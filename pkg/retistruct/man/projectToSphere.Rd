% Generated by roxygen2 (4.1.0): do not edit by hand
% Please edit documentation in R/spheristruct.R
\name{projectToSphere}
\alias{projectToSphere}
\title{Project mesh points in the flat outline onto a sphere}
\usage{
projectToSphere(r)
}
\arguments{
\item{r}{\code{Outline} object to which the following information
has been added with \code{\link{mergePointsEdges}}:
\describe{
\item{\code{Pt}}{The mesh point coordinates.}
\item{\code{Rsett}}{The set of points on the rim.}
\item{\code{A.tot}}{The area of the flat outline.}}}
}
\value{
\code{reconstructedOutline} object containing the
following extra information
\item{\code{phi}}{Lattitude of mesh points.}
\item{\code{lmabda}}{Longitude of mesh points.}
\item{\code{R}}{Radius of sphere.}
}
\description{
This takes the mesh points from the flat outline and maps them to
the curtailed sphere. It uses the area of the flat outline and
\code{phi0} to determine the radius \code{R} of the sphere. It
tries to get a good first approximation by using the function
\code{\link{stretchMesh}}.
}
\author{
David Sterratt
}

