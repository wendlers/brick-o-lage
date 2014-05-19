include <../../lib/Lego_dimensions.scad>;
include <../../lib/Lego_klotz.scad>;
include <../../lib/Lego_technic_beam.scad>;

// outer size, must be a multible of brick-size
bw=7;
bl=5;
w1=brick_w * bw;
l1=brick_w * bl;
h1=plate_h * 3;

// inner size, exactely the board size
w2=46.5;
l2=37.5;
h2=h1;

// no play between legos
play=0;

// cover(w1, l1, plate_h, (w1 - w2) / 2, w1, h1, l1, w2, l2, h2, bw, bl);
// case(w1, l1, h1, w2, l2, h2, bw, bl, "LEGO_TECHNIC", 0);

// show_exploded("LEGO_TECHNIC");
// show_exploded("NONE");
// show_closed(1, "LEGO_TECHNIC");
// show_closed(1, "NONE");

// print_case("LEGO_TECHNIC");
// print_case("NONE");
print_cover();

// translate([0, 0, plate_h / 2])
//	pcb(w2, l2, 1);


module print_case(withBase)
{
	if(withBase == "LEGO")
	{
		translate([0, 0, plate_h * 2])
			case(w1, l1, h1, w2, l2, h2, bw, bl, withBase, 0);
	}
	else
	{
		translate([0, 0, plate_h])
			case(w1, l1, h1, w2, l2, h2, bw, bl, withBase, 0);
	}
}

module print_cover()
{
	rotate([180, 0, 0])
		translate([0, 0, -plate_h * 3])
			cover(w1, l1, plate_h, (w1 - w2) / 2, w1, h1, l1, w2, l2, h2, bw, bl);
}

module show_exploded(withBase)
{
	color("red")
	{
		case(w1, l1, h1, w2, l2, h2, bw, bl, withBase, 0);

		translate([0, 0, plate_h * 8])
			cover(w1, l1, plate_h, (w1 - w2) / 2, w1, h1, l1, w2, l2, h2, bw, bl);
	}

	translate([0, 0, plate_h * 5])
		pcb(w2, l2);
}

module show_closed(withPcb, withBase)
{
	color("red")
	{
		case(w1, l1, h1, w2, l2, h2, bw, bl, withBase, 0);
		cover(w1, l1, plate_h, (w1 - w2) / 2, w1, h1, l1, w2, l2, h2, bw, bl);
	}

	if(withPcb == 1)
	{
		translate([0, 0, plate_h / 2 - 2])
			pcb(w2, l2);
	}
}

module case(w1, l1, h1, w2, l2, h2, bw, bl, withBase, noSnapin)
{
	difference()
	{
	    case_outer(w1, l1, h1, bw, bl, withBase);
		case_inner(w2, l2, h2);
		translate([0, 0, 3])
			case_inner(w1-2, l2, h2-2);

 		translate([0, 0, plate_h / 2 - 2])
			pcb(w2, l2, 1);

		if(noSnapin == 0)
			cover_snapin_cutout(l1, plate_h, h1);		
	}

	/* stand-offs to hold PCB in Place */

	translate([0, -l2 / 2 + 1, -1.75])
		cube([w2, 2, 2], center=true);

	translate([0, +l2 / 2 - 1, -1.75])
		cube([w2, 2, 2], center=true);
}

module case_outer(w, l, h, bw, bl, withBase)
{
	union()
	{
		translate([0, 0, plate_h * h / plate_h / 2 - plate_h])
			cube([w, l, h], center=true);

		if(withBase == "LEGO_TECHNIC")
		{
				mirror([0, 0, 0])
					translate([-w1/2 - 7.8, -l1/2, -(2 * plate_h - 7.5)/2])
						rotate([0, 90, 0])
							lego_technic_beam(1);
	
				mirror([0, 1, 0])
					translate([-w1/2 - 7.8, -l1/2, -(2 * plate_h - 7.5)/2])
						rotate([0, 90, 0])
							lego_technic_beam(1);
	
				mirror([1, 0, 0])
					translate([-w1/2 - 7.8, -l1/2, -(2 * plate_h - 7.5)/2])
						rotate([0, 90, 0])
							lego_technic_beam(1);
	
				mirror([0, 1, 0])
					mirror([1, 0, 0])
						translate([-w1/2 - 7.8, -l1/2, -(2 * plate_h - 7.5)/2])
							rotate([0, 90, 0])
								lego_technic_beam(1);
	
		}
		else if(withBase == "LEGO")
		{
			translate([0, 0, -plate_h])
				KLOTZ(bw, bl, 1, Tile=true);
		}
	}
}

module case_inner(w, l, h)
{
	translate([0, 0, plate_h * h / plate_h / 2 - 2])
		cube([w, l, h], center=true);	
}

module io_spacer(w, l, h)
{
	translate([0, 0, plate_h * h / plate_h / 2 + 1])
		cube([w, l, h], center=true);	
}

module cover(w, l, h, t, w1, h1, l1, w2, l2, h2, bw, bl, withNoppen)
{
	union() 
	{
		difference()
		{
			translate([0, 0, h1])
			{
				union()
				{
					translate([0, 0, -h / 2])
					{
						difference()
						{
							translate([0, 0, -h/2])
								cube([w1, l1, 2 * h], center=true);
	
							translate([0, 0, -h/2 - 4])
								cube([w1 - 4, l1 - 4, 4*h], center=true);
	
							translate([-w2/2 + 5, +l2/2 - 10 - (l1-l2)/2,  -h - 2 * t])
	  							io_spacer(10, 20, plate_h * 5);
	
						}
						translate([0, -l2/2, -6])
							cube([7, 2, 3], center=true);
	
						mirror([0, 1, 0])
							translate([0, -l2/2, -6])
								cube([7, 2, 3], center=true);
					}
					if(withNoppen == 1)
					{
						difference()
						{
					
							KLOTZ(bw, bl, 0, Tile=false);
				
							translate([- 3, l1 / 2 - brick_w / 2, - h * 2])
								io_spacer(w1 - 3, brick_w + 1, plate_h * 3);
						}
					}

					/* stand-offs to hold PCB in place */
					translate([-w2 / 2 + 14, -l2 / 2 + 13.5, -5])
						cylinder(r=3, h=5, $fn=100);	

					translate([-w2 / 2 + 27, -l2 / 2 + 13.5, -5])
						cylinder(r=3, h=5, $fn=100);	
				}
			}
			case(w1, l1, h1, w2, l2, h2, bw, bl, "NONE", 1);
			pcb(w2, l2, 2);
		}

		translate([0, -l2/2 - 1, h1-6.5-h/2])
		{
			translate([0, 1, 0])
				cube([7, 2, 2], center=true);

			rotate([90, 0, 90])
				cylinder(h=7, r=1, center=true, $fn=50);
		}

		mirror([0, 1, 0])
			translate([0, -l2/2 - 1, h1-6.5-h/2])
			{
				translate([0, 1, 0])
				cube([7, 2, 2], center=true);
	
				rotate([90, 0, 90])
					cylinder(h=7, r=1, center=true, $fn=50);
			}
	}
}

module cover_snapin_cutout(l, h, h1)
{
	translate([0, 0, h1-6.5-h/2])
		cube([7.2, l*1.5, 2.2], center=true);
}

module pcb(w, l, withCutouts)
{
	union()
	{
		// pcb
		color("silver")
			cube([w, l, 1], center=true);

		// ic1 socket
		color("DimGray")
			translate([-4, -5.25, 2.5])
				cube([25, 10, 5], center=true);

		// ic1
		color("black")
			translate([-4, -5.25, 5 + 1.5])
				cube([23, 8, 3], center=true);

		// ic2 socket
		color("DimGray")
			translate([-1, +9.25, 2.5])
				cube([20, 10, 5], center=true);

		// ic1
		color("black")
			translate([-1, +9.25, 5 + 1.5])
				cube([17, 8, 3], center=true);

		// led1
		color("blue")
			translate([w / 2 - 3.5, - l / 2 + 3, 0])
				cylinder(h=5, r=1.5);

		// led2
		color("green")
			translate([w / 2 - 9.5, - l / 2 + 3, 0])
				cylinder(h=5, r=1.5);

		if( withCutouts > 0)
		{
/*
			// led1 cutout
			color("blue")	
				translate([w / 2 - 4.5, - l / 2 + 3, 4])
					rotate([90, 0, 0])		
						cylinder(h=7, r=1.5, $fn=100);

			// led2 cutout
			color("green")
				translate([w / 2 - 9.5, - l / 2 + 3, 4])
					rotate([90, 0, 0])		
						cylinder(h=7, r=1.5, $fn=100);
*/
			if( withCutouts == 1 )
			{
				// prog+power-cable cutout
				color("yellow")
					translate([w / 2 + 15 / 2 - 5, -1, 5.5])
						cube([15, 22, 10], center=true);
	
				// i2c-cable cutout
				color("cyan")
					translate([-w / 2 - 15 / 2 + 5, -6.5, 5.5])
						cube([15, 8, 10], center=true);		
			}
		}
	}
}
