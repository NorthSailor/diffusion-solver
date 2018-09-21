#include "xdmf-generator.h"

void write_xdmf(FILE *fp, int grid_width, int grid_height)
{
	const char *format_string =
		"<?xml version=\"1.0\" ?>\n"
		"<!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\" []>\n"
		"<Xdmf>\n"
		"\t<Domain>\n"
		"\t\t<Grid Name=\"Plate\">\n"
		"\t\t\t<Topology TopologyType=\"2DCoRectMesh\" Dimensions=\"%d %d\">\n"
		"\t\t\t</Topology>\n"
		"\t\t\t<Geometry GeometryType=\"ORIGIN_DXDY\">\n"
		"\t\t\t\t<DataItem Format=\"XML\" Dimensions=\"2\">\n"
		"\t\t\t\t\t0.0 0.0\n"
		"\t\t\t\t</DataItem>\n"
		"\t\t\t\t<DataItem Format=\"XML\" Dimensions=\"2\">\n"
		"\t\t\t\t\t1.0 1.0\n"
		"\t\t\t\t</DataItem>\n"
		"\t\t\t</Geometry>\n"
		"\t\t\t<Attribute Name=\"Potential\" AttributeType=\"Scalar\" Center=\"Node\">\n"
		"\t\t\t\t<DataItem Format=\"Binary\" Dimensions=\"%d %d\"\n"
		"\t\t\t\t\tNumberType=\"Float\" Precision=\"4\">\n"
		"\t\t\t\t\tpotential.bin\n"
		"\t\t\t\t</DataItem>\n"
		"\t\t\t</Attribute>\n"
		"\t\t\t<Attribute Name=\"Flux\" AttributeType=\"Vector\" Center=\"Node\">\n"
		"\t\t\t\t<DataItem Format=\"Binary\" Dimensions=\"%d %d 2\"\n"
		"\t\t\t\t\tNumberType=\"Float\" Precision=\"4\">\n"
		"\t\t\t\t\tflux.bin\n"
		"\t\t\t\t</DataItem>\n"
		"\t\t\t</Attribute>\n"
		"\t\t</Grid>\n"
		"\t</Domain>\n"
		"</Xdmf>\n";
	fprintf(fp, format_string, grid_width, grid_height, grid_width,
		grid_height, grid_width, grid_height);
}