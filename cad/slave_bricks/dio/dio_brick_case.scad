include <../../lib/Lego_dimensions.scad>;
include <../../lib/Lego_klotz.scad>;

// outer size, must be a multible of brick-size
bw=6;
bl=4;
w1=brick_w * bw;
l1=brick_w * bl;
h1=plate_h * 4;

// inner size, exactely the board size
w2=45;
l2=28;
h2=h1;

// no play between legos
play=0;

show_exploded("LEGO");
// show_exploded("NONE");
// show_closed(1, "LEGO");
// show_closed(1, "NONE");

// print_case("LEGO");
// print_case("NONE");
// print_cover();

// translate([0, 0, plate_h / 2])
// pcb(w2, l2, 1);


module print_case(withBase)
{
	if(withBase == "LEGO")
	{
		translate([0, 0, plate_h * 2])
			case(w1, l1, h1, w2, l2, h2, bw, bl, withBase);
	}
	else
	{
		translate([0, 0, plate_h])
			case(w1, l1, h1, w2, l2, h2, bw, bl, withBase);
	}
}

module print_cover()
{
	rotate([180, 0, 0])
		translate([0, 0, -plate_h * 4])
			cover(w1, l1, plate_h, (w1 - w2) / 2, w1, h1, l1, w2, l2, h2, bw, bl);
}

module show_exploded(withBase)
{
	color("red")
	{
		case(w1, l1, h1, w2, l2, h2, bw, bl, withBase);

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
		case(w1, l1, h1, w2, l2, h2, bw, bl, withBase);
	}
	color("red")
	{
		cover(w1, l1, plate_h, (w1 - w2) / 2, w1, h1, l1, w2, l2, h2, bw, bl);
	}

	if(withPcb == 1)
	{
		translate([0, 0, plate_h / 2 - 2])
			pcb(w2, l2);
	}
}

module case(w1, l1, h1, w2, l2, h2, bw, bl, withBase)
{
	difference()
	{
		case_outer(w1, l1, h1, bw, bl, withBase);
		case_inner(w2, l2, h2);
		translate([- 3, l1 / 2 - brick_w / 2, plate_h / 2 - plate_h + 1])
			io_spacer(w1 - 3, brick_w + 1, plate_h * 5);
 		translate([0, 0, plate_h / 2 - 2])
			pcb(w2, l2, 1);
	}

	/* stand-offs to hold PCB in Place */

	translate([-w2 / 2 + 2, -l2 / 2 + 2, -1.75])
		cube([4, 4, 2], center=true);

	translate([-w2 / 2 + 30.5, -l2 / 2 + 2, -1.75])
		cube([4, 4, 2], center=true);

	translate([w2 / 2 - 6, -1, -1.75])
		cube([4, 4, 2], center=true);

	translate([-w2 / 2 + 2, l2 / 2 - 9.5, -1.75])
		cube([4, 4, 2], center=true);
}

module case_outer(w, l, h, bw, bl, withBase)
{
	translate([0, 0, plate_h * h / plate_h / 2 - plate_h])
		cube([w, l, h], center=true);

	if(withBase == "LEGO")
	{
		translate([0, 0, -plate_h])
			KLOTZ(bw, bl, 1, Tile=true);
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
						cube([w, l, h], center=true);

						translate([- 3, l1 / 2 - brick_w / 2, - h * 2])
							io_spacer(w1 - 3, brick_w + 1, plate_h * 3);
					}
				
				
					difference()
					{
						translate([0, 0, - 2 * t])
							cube([w - 2 * t, l - 2 * t, 2 * t], center=true);

						translate([0, 1, - 2 * t])
							cube([w - 3 * t, l - 3 * t + 1, 2 * t + 4], center=true);

						translate([- 3, l1 / 2 - brick_w / 2, - h * 2])
							io_spacer(w1 - 3, brick_w + 1, plate_h * 3);
					}

					translate([w / 2 - t / 2, 0, - 2 * t])
						cube([t, l1, 2 * t], center=true);

					translate([- w / 2 + t / 2, 0, - 2 * t])
						cube([t, l1, 2 * t], center=true);

					/* stand-offs to hold PCB in place */

					translate([-w2 / 2 + 14, -l2 / 2 + 12.5, -plate_h - 0.2])
						cylinder(r=3, h=2.2, $fn=100);	

					translate([-w2 / 2 + 27, -l2 / 2 + 12.5, -plate_h - 0.2])
						cylinder(r=3, h=2.2, $fn=100);				
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
			}
		}

		case(w1, l1, h1, w2, l2, h2, bw, bl, "NONE");
			translate([- 3, l1 / 2 - brick_w / 2, plate_h / 2 - plate_h + 1])
				io_spacer(w1 - 3, brick_w + 1, plate_h * 5);
	}
}

module pcb(w, l, withCutouts)
{
	union()
	{
		// pcb
		color("silver")
			cube([w, l, 1], center=true);

		// ic socket
		color("DimGray")
			translate([-4, -2, 2.5])
				cube([25, 10, 5], center=true);

		// ic
		color("black")
			translate([-4, -2, 5 + 1.5])
				cube([23, 8, 3], center=true);

		// led1
		color("blue")
			translate([w / 2 - 3, - l / 2 + 3, 0])
				cylinder(h=5, r=1.5);

		// led2
		color("green")
			translate([w / 2 - 8, - l / 2 + 3, 0])
				cylinder(h=5, r=1.5);

		if( withCutouts == 1)
		{
			// led1 cutout
			color("blue")	
				translate([w / 2 - 3, - l / 2 + 3, 4])
					rotate([90, 0, 0])		
						cylinder(h=7, r=1.5, $fn=100);

			// led2 cutout
			color("green")
				translate([w / 2 - 8, - l / 2 + 3, 4])
					rotate([90, 0, 0])		
						cylinder(h=7, r=1.5, $fn=100);

			// prog-cable cutout
			color("yellow")
				translate([w / 2 + 15 / 2 - 5, l / 2 - 16 / 2 - 7, 5.5])
					cube([15, 18, 10], center=true);
		
			// power-cable cutout
			color("green")
				translate([w / 2 + 15 / 2 - 5, l / 2 - 6 / 2, 5.5])
					cube([15, 6, 10], center=true);

			// i2c-cable cutout
			color("cyan")
				translate([-w / 2 - 15 / 2 + 5, l / 2 - 9 / 2 - 12.7, 5.5])
					cube([15, 9, 10], center=true);
		}
	}
}
