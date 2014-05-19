/** 
 * Parametric Lego Technic Beam
 * 
 * Based on projunk's thing: 
 *
 * http://www.thingiverse.com/thing:203935
 *
 */

// lego_technic_beam(4);

module lego_technic_beam(NrOfHoles)
{
	Pitch = 8;
	Radius1 = 5.0 / 2;
	Radius2 = 6.2 / 2;
	Height = 7.8;
	Depth = 0.8;
	Width = 7.5;
	MidThickness = 2;

    Length = (NrOfHoles - 1) * Pitch;
	Thickness = (Width - 2 * Radius2) / 2;

	difference()
	{
		union()
		{			
			cube([Length, Thickness, Height]);
			translate([0, Width-Thickness,0]) 
				cube([Length, Thickness, Height]);
			translate([0, 0, Height/2-MidThickness/2]) 
				cube([Length, Width, MidThickness]);
			for (i = [1:NrOfHoles])
			{
				translate([(i-1)*Pitch, Width/2,0]) 
				{
					cylinder(r=Width/2, h=Height, $fn=50);
				}
			}
		}

		union()
		{
			for (i = [1:NrOfHoles])
			{
				translate([(i-1)*Pitch, Width/2,0]) 
				{
					cylinder(r=Radius2,h=Depth, $fn=50);
					cylinder(r=Radius1,h=Height, $fn=50);
					translate([0,0,Height-Depth]) 
						cylinder(r=Radius2,h=Depth, $fn=50);
				}
			}
		}
	}
}



