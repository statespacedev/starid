nasa skymap sky2000 is the foundation here and it's got some interesting history. back in the day i interacted with some of the people involved and learned a bit. there were roughly two phases of skymap's history - before hubble, and after. nasa still had power in those days, and when it turned out after launch that star catalogs were among hubble's problems, resources were allocated to improve skymap. serious work was done in the 90s to make it the baseline catalog for spacecraft star trackers.

here are some notes from back in the day, these are floating around on the net and turn up with a quick googling. details about the hubble events, and an interesting read, are in

[The Hubble Wars: Astrophysics Meets Astropolitics in the Two-Billion-Dollar Struggle over the Hubble Space Telescope, Eric J. Chaisson](http://a.co/8E8tqBU)

## note

The SKYMAP Master Catalog, Myers J.R., Sande C.B., Miller A.C., Warren Jr. W.H., Tracewell D.A., Goddard Space Flight Center, Flight Dynamics Division (2002)

The SKYMAP Star Catalog System consists of a Master Catalog stellar database and a collection of utility software designed to create and maintain the database and to generate derivative mission star catalogs (run catalogs). It contains an extensive compilation of information on almost 300000 stars brighter than magnitude 8.0.

The original SKYMAP Master Catalog was generated in the early 1970's. Incremental updates and corrections were made over the following years but the first complete revision of the source data occurred with Version 4.0. This revision also produced a unique, consolidated source of astrometric information which can be used by the astronomical community. The derived quantities were removed and wideband and photometric data in the R (red) and I (infrared) systems were added.

Version 4 of the SKY2000 Master Catalog was completed in April 2002; it marks the global replacement of the variability identifier and variability data fields. More details can be found in the description file sky2kv4.pdf.

The SKY2000 Version 5 Revision 4 Master Catalog differs from Revision 3 in that MK and HD spectral types have been added from the Catalogue of Stellar Spectral Classifications (B. A. Skiff of Lowell Observatory, 2005), which has been assigned source code 50 in this process. 9622 entries now have MK types from this source, while 3976 entries have HD types from this source. SKY2000 V5 R4 also differs globally from preceding MC versions in that the Galactic coordinate computations performed by UPDATE have been increased in accuracy, so that differences from the same quantities from other sources are now typically in the last decimal places carried in the MC.

## note

V/77 skymap Star Catalog - Version 3.7 (Slater+ 1992), skymap, Revision 3 - Update 1 (version 3.7), Slater M., and Hashmall J., NASA/Goddard Space Flight Center Document 554-FDD-89/001R3UD1 (1992)

The SKYMAP system was developed to provide an accurate and complete catalog of all stars with blue or visual magnitudes brighter than 9.0 for use by attitude determination programs. The catalog contains approximately 248560 stars. Because for this use, completeness is more important than high accuracy, many quantities are little more than good guesses. The catalog contains data for the following four purposes:

Cross-referencing stars to other catalogs
Calculating star instrumental magnitudes
Calculating star positions at a given epoch
Determining star position errors, distances to near neighbors, and magnitude variability.
Introduction:

The original version has been improved and updated with corrections several times. Version 3.7 incorporates known errors documented previously in SKYMAP Error Reports. Magnitude errors discovered in SKYMAP Version 3.6 were corrected. Other minor catalog corrections, including the deletion of five duplicate entries and the addition of two stars, were also performed. The original catalog was compiled by D. M. Gottlieb of Computer Sciences Corporation (CSC) under contract to NASA. The compilation and statistics of the catalog are described by Gottlieb (1978), while the source referenced above describes in detail the contents of the current version and the derivation of values not available observationally.

## note

interesting note about the catalogs used to generate the initial catalog for the gaia mission.

Hipparcos (Perryman et al. 1997): The photometry from the original Hipparcos Catalogue and the astrometric parameters from the update by van Leeuwen (2007) when published. Initially, all entries were included regardless of the known errors, e.g., also for entries that are considered erroneous. Since inclusion in the ASC requires an estimate of the G-mag, the unreal entries were excluded as part of the cleaning phase.

Tycho-2 (Høg et al. 2000): This catalogue forms the backbone of all the major ground-based catalogues currently available. It was made from a combination of the Tycho star mapper observations on the Hipparcos satellite (Høg et al. 1997), the Astrographic Catalogue, and 143 other ground-based catalogues.

Sky2000 (Myers et al. 2001): The SKYMAP Star Catalogue System is a list of all stars with either measured Johnson blue or visual magnitudes brighter than 9.0. The version used here had 299 167 entries of which 212 were not in the combined Hipparcos + Tycho-2 catalogues. Sky2000 provides positions at J2000, proper motions, and a blue and visual magnitude. We assumed positions errors of 100 mas, proper motions error of 10 mas yr−1, and ASC magnitudes derived from Sky2000 entries to have errors of 0.6.

UCAC4 (Zacharias et al. 2013): The USNO CCD Astrograph Catalogue version 4 was the most precise all-sky astrometric catalogue (at the time of constructing the catalogue) in the range V=10–16 mag currently available. There are no original standard magnitudes in this catalogue.

GSC2.3 (Lasker et al. 2008): The Second Guide Star Catalogue version 2.3 forms the bulk of the photometry and defines the red and blue magnitudes (BJ and RF) as this has the largest sky coverage with a homogeneous photometric system. The only variation with the public version is that we removed the multiple entries discussed in Section 4.2 of Lasker et al. (2008). This was done by insisting that only one entry from any objects with position differences of less than 10 mas were kept, selecting Tycho-2 or Sky2000 over other entries.

PPMXL (Roeser et al. 2010): The Positions and Proper Motions ‘Extra Large’ (XL) Catalogue was produced from a combination of the USNO–B (Monet et al. 2003) and the Two Micron Sky Survey point source catalogue (Epchtein et al. 1999). This catalogue was included to provide magnitudes for those entries that did not have them in the previous catalogues.

