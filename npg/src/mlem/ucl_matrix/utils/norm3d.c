norm3d(viewnum, nviews, zelem, zSize) {
	int curview, z, elem;
	for (curview=viewnum; curview<viewnum+nviews; curview++) {
		 for (z=zelem; z<zelem+zSize; z++) {
			  start_view = curview * mash_;
			  end_view = start_view + mash_;

/* set the geometric profile and interference pattern profile pointers */
			  if (num_geo_corr_planes > 1) geoptr = &geocorr[z*nelems_];
			  else geoptr = geocorr;

/* set the pointer to interference pattern array */
			  interfcorrptr = interfcorr;

/* process all the elements	*/
			  for (elem = -nelems_/2; elem<nelems_/2; elem++) {
					view_efficiency = 0.0;

/* if the views were mashed, take into account the efficiencies
of all the crystals */
					for (int view=start_view; view<end_view; view++) {
						 lor_efficiency = 0.0;
						 crys_a = view_element_to_det1 (elem, view, nviews_);
						 crys_b = view_element_to_det2 (elem, view, nviews_);
						 for (int pair=1; pair<=npairs*2; pair += 2)
							  lor_efficiency += cryseff[crys_a + ncrys_*ringA] *
													  cryseff[crys_b + ncrys_*ringB];
						 view_efficiency += lor_efficiency * 
												  interfcorrptr[view % ntxdets_];
					}

					dinom = view_efficiency * (*geoptr++);

					numer = *projptr++ * sf;

					if (dinom) *normprojptr++ = numer / dinom;
					else *normprojptr++ = 1.0;

					interfcorrptr += ntxdets_;
			  } /* for elem */
		 } /* for z */
	} /* for curview */
}
