include <../../lib/Lego_dimensions.scad>;
include <../../lib/Lego_klotz.scad>;
include <../../lib/Lego_technic_beam.scad>;

// pilz();
// cover();
// case();
// contact_switch(1);
// contact_switch_bearing();

w=brick_w * 8;

// print_case();
// print_cover();

// show_case_exploded();
show_case_closed();

// contact_switch(1);
// contact_switch_bearing();

/*
translate([0, 0, +25])
	color("red")
		KLOTZ(2 * 4, 2 * 4, 1);

translate([0, 0, -25])
	color("red")
		KLOTZ(2 * 4, 2 * 4, 1);
*/


module show_case_closed()
{
	pilz();

	color("orange")	
		cover();

	color("blue")
		case();
}

module show_case_exploded()
{
	translate([0, 0, 100])
		pilz();

	translate([0, 0, 60])
		color("orange")	
			cover();

	color("blue")
		case();

	translate([0, 0, -40])
		contact_switch();
}

module print_cover()
{
	translate([0, 0, 21.5])
		rotate([180, 0, 0])
			cover();
}

module print_case()
{
	translate([0, 0, 24])
			case();
}

module pilz()
{
	color("red")
		translate([0, 0, 21.5 + 10 + 4.0])
			cylinder(r=40.0/2, h=6.5, $fn=100);

	color("red")
		translate([0, 0, 21.5 + 10])
			cylinder(r=24.0/2, h=4.0, $fn=100);

	color("silver")
		translate([0, 0, 21.5 + 5])
			cylinder(r=30.0/2, h=5.0, $fn=100);

	color("black")
		translate([0, 0, 21.5])
			cylinder(r=30.0/2, h=5.0, $fn=100);

	difference()
	{
		color("black")
			translate([0, 0, 10.7])
				cylinder(r=23/2, h=21.5 - 10.7, $fn=100);

		translate([23/2, 0,  10.7 + (21.6 - 10.7) / 2])
			cube([2.5, 3.2, 21.5 - 10.7 + 1], center=true);
	}

	difference() 
	{
		color("black")
			cylinder(r=17.5/2, h=10.7, $fn=100);

		translate([0, 0, -1])
			cylinder(r=13.4/2, h=11.7, $fn=100);

		translate([-7.2/2, - 18/2, -1])
			cube([7.2, 18.0, 6.5]);
	}
}

module cover()
{
	h1 = 6 + 2.5 + 6.3 + 2.7 + 3.5 + 24.5;

	union()
	{
		difference()
		{
			translate([0, 0, 21.5 - 1.1])
				cube([w - 2, w - 2, 2], center=true);

			pilz();
			drill_wholes();
			translate([0, 48/2, 18])
				cylinder(r=5/2, h=6, $fn=100);
		}
		
		difference()
		{
			translate([0, 0, -h1 + 21.5 + 2.5])
				cylinder(r=40/2, h=h1-3, $fn=100);

			translate([0, 0, -h1 + 21.5 + 2])
				cylinder(r=40/2 - 3, h1 - 1, $fn=100);		
			
			translate([0, 0, -h1 + h1*0.7])
				cube([20, 50, h1], center=true);

			pilz();
		}
	}
}

module drill_wholes()
{
	translate([61/2 - 8/2, 61/2 - 8/2, -3])
	{
		cylinder(r=3.6/2, h=30, $fn=100);
	}
		
	mirror([1, 0 , 0])
		translate([61/2 - 8/2, 61/2 - 8/2, -3])
		{
			cylinder(r=3.6/2, h=30, $fn=100);
		}

	mirror([0, 1 , 0])
		translate([61/2 - 8/2, 61/2 - 8/2, -3])
		{
			cylinder(r=3.6/2, h=30, $fn=100);
		}

	mirror([1, 1 , 0])
		translate([61/2 - 8/2, 61/2 - 8/2, -3])
		{
			cylinder(r=3.6/2, h=30, $fn=100);
		}
}

module cover_mounting()
{
	h1 = 6 + 2.5 + 6.3 + 2.7 + 3.5 + 24.5;
	h2 = h1 - 3;

	translate([61/2 - 8/2, 61/2 - 8/2, -(h2)/2 - 1.8])
	{
		difference()
		{
			cylinder(r=8/2, h=h2, $fn=100);
			cylinder(r=2.8/2, h=h2+1, $fn=100);
			rotate([0, 0, 135])
				translate([-1/2, 0, 0])
					cube([1, 6, h2+1]);

			contact_switch(1);
		}
	}
}

module lego_technic_connectors()
{
	translate([-w/2, -2.5 * brick_w, -24.2 + brick_w/2])
		rotate([0, 90, 0])
			lego_technic_beam(1);

	mirror([0, 1, 0])
		translate([-w/2, -2.5 * brick_w, -24.2 + brick_w/2])
			rotate([0, 90, 0])
				lego_technic_beam(1);
}

module lego_technic_connectors_cutout()
{
	translate([-w/2 - 1, -2.5 * brick_w + (brick_w/2 - 0.3), -24.2 + brick_w/2])
		rotate([0, 90, 0])
			cylinder(r=brick_w/2 - 0.3, h=brick_w + 1, $fn=100);

	mirror([0, 1, 0])
		translate([-w/2 - 1, -2.5 * brick_w + (brick_w/2 - 0.3), -24.2 + brick_w/2])
			rotate([0, 90, 0])
				cylinder(r=brick_w/2 - 0.3, h=brick_w + 1, $fn=100);
}
	
module case()
{
	h1 = 6 + 2.5 + 6.3 + 2.7 + 3.5 + 24.5;
	h2 = h1 - 3;

	union()
	{
		difference()
		{
			translate([0, 0, 24.5 - h1/2 - 3])
				cube([w, w, h1], center=true);

			translate([0, 0, 24.5 - h2/2 - 3])		
				cube([58, 58, h2 + 1], center=true);		

			cover();
			contact_switch(1);

			translate([0, 58/2 + 5, - 19])
				rotate([90, 0, 0])
					cylinder(r=4/2, h=6, $fn=100);

			lego_technic_connectors_cutout();

			mirror([1, 0, 0])
				lego_technic_connectors_cutout();

			mirror([1, 1, 0])
				lego_technic_connectors_cutout();

			rotate([0, 0, 180])
				mirror([1, 1, 0])
					lego_technic_connectors_cutout();
		}

		contact_switch_bearing();
		cover_mounting();
			
		mirror([1, 0 , 0])
			cover_mounting();

		mirror([0, 1 , 0])
			cover_mounting();

		mirror([1, 1 , 0])
			cover_mounting();

		difference()
		{
			translate([0, 0, -h1 + 21.5 + 2])
				cylinder(r=45/2, 5, $fn=100);

			translate([0, 0, -h1 + 21.5 + 1])
				cylinder(r=40/2 - 2.8, 7, $fn=100);

			cover();

			translate([0, 58/2 - 4, - 19])
				rotate([90, 0, 0])
					cylinder(r=3/2, h=10, $fn=100);
		}

		lego_technic_connectors();

		mirror([1, 0, 0])
			lego_technic_connectors();

		mirror([1, 1, 0])
			lego_technic_connectors();

		rotate([0, 0, 180])
			mirror([1, 1, 0])
				lego_technic_connectors();
	}
}

module contact_switch_bearing()
{
	difference()
	{
		translate([0, 0, -(6 + 2.5 + 6.3 + 2.7 + 3.5 + 1)])
			cylinder(r=9.5/2+3, h=16.0, $fn=100);
		contact_switch(1);
	}
}

module contact_switch(for_diff)
{
	translate([0, 0, -(6 + 2.5 + 6.3 + 2.7 + 3.5) + 5.0])
	{
		translate([0, 0, 6 + 2.5 + 6.3 + 2.7])
			color("green")
				cylinder(r=5.8/2, 3.5, $fn=100);
	
		translate([0, 0, 6 + 2.5 + 6.3])
			color("silver")
				cylinder(r=4.5/2, 2.7, $fn=100);
	
		if(for_diff)
		{
			translate([0, 0, 6 + 2.5])
					cylinder(r=8/2, h=6.3, $fn=100);

			translate([0, 0, 6])
					cylinder(r=10.5/2, h=2.5, $fn=100);
	
			translate([0, 0, -14])
				cylinder(r=10.5/2, h=20, $fn=100);
		}
		else
		{
			translate([0, 0, 6 + 2.5])
				color("silver")
					cylinder(r=7/2, h=6.3, $fn=100);

			translate([0, 0, 6])
				color("silver")
					cylinder(r=9.5/2, h=2.5, $fn=100);

			color("black")
				cylinder(r=7/2, h=6, $fn=100);
		}
	
		translate([2, -5/2, -10])
			color("silver")
				cube([10, 5, 10]);
	
		mirror([1, 0, 0])
			translate([2, -5/2, -10])
				color("silver")
					cube([10, 5, 10]);
	}
}

