#include "utils.cpp"

#include "math.cpp"
#include "fourier.cpp"

int main(int argc, char *argv[]) {
	//readme
	if (argc < 2) {
		cout << "Fourier filtering course work by Lev\n"
		        "File operations provided by stb_image and stb_image_write (public domain, check git.io/s1PH6w for more info)\n\n"
		        "Usage:\n"
		        "Specify input filename (and optionally output filename) as commandline parameter, or drag-n-drop file to this executable.\n\n"
		        "Supported file formats:\n"
		        "Input:\n"
		        "  JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib)\n"
		        "  PNG 1/2/4/8-bit-per-channel (16 bpc not supported)\n"
		        "  TGA (not sure what subset, if a subset)\n"
		        "  BMP non-1bpp, non-RLE\n"
		        "  PSD (composited view only, no extra channels, 8/16 bit-per-channel)\n"
		        "  GIF (*comp always reports as 4-channel)\n"
		        "  HDR (radiance rgbE format)\n"
		        "  PIC (Softimage PIC)\n"
		        "  PNM (PPM and PGM binary only)\n\n"
		        "Output:\n"
		        "  PNG (default)\n"
		        "  BMP\n"
		        "  TGA\n"
		        "  HDR\n"
		;
		return 0;
	}

	//loading image, showing some info
	string filename = argv[1];
	auto image = load_image(filename);
	auto N = image.height;
	auto M = image.width;
	cout << filename << ": " << M << "x" << N << "x" << image.depth << "\n"
	     << (is_power_of_2(N) ? "fft" : "dft") << " will be used for columns\n"
	     << (is_power_of_2(M) ? "fft" : "dft") << " will be used for rows\n"
	;

	//creating new folder and stuff
	string outputFilename;

	if (argc < 3)
		outputFilename = filename.substr(0, filename.rfind('.')) + ".png";
	else 
		outputFilename = argv[2];
	
	auto found = outputFilename.find_last_of("/\\");
	string outputFolder = outputFilename.substr(0, found+1);
	outputFolder += string("fourier") + outputFilename[found];

	create_dir(outputFolder);

	outputFilename = outputFolder+outputFilename.substr(found+1);

	//ask user for radius
	float radius;
	cout << "radius (from 0 to 1) = "; cin >> radius;
	radius = min(N,M)*radius/2;

	auto complexImage = to_Complex(image);
	destroy(image);

	cout << "Fourier transform";
	Timer t;
	auto complexImageDFT = fourier(complexImage);
	cout << " done in " << t.leap() << "ms\n";
	destroy(complexImage);
	defer(destroy(complexImageDFT));

	auto output = [outputFilename](Image<Complex> inComplexImageDFT, auto H, string filterName){
		auto complexImageDFT = copy(inComplexImageDFT);
		defer(destroy(complexImageDFT));

		cout << "\nFilter " << filterName;
		Timer t;
		for (int k = 0; k < complexImageDFT.depth; ++k) {
			auto& channel = complexImageDFT.channels[k];
			for (int i = 0; i < channel.height; ++i)
				for (int j = 0; j < channel.width; ++j)
					channel.rows[i][j] *= H(i, j);
		}

		cout << " applied in " << t.leap() << "ms\n";
		if (!filterName.empty())
			filterName = "_"+ filterName;

		auto byteImageDFT = to_byte(complexImageDFT);
		write_image(byteImageDFT, add_postfix(outputFilename, filterName+"_spectrum"));
		destroy(byteImageDFT);

		cout << "Inverted Fourier transform";
		t.leap();//reset clock
		auto complexImageRestored = inv_fourier(complexImageDFT);
		cout << " done in " << t.leap() << "ms\n";

		auto byteImageRestored = to_byte(complexImageRestored);
		destroy(complexImageRestored);
		write_image(byteImageRestored, add_postfix(outputFilename, filterName+"_restored"));
		destroy(byteImageRestored);
	};

	//distance to center
	auto D = [&](float u, float v) {
		return sqrt(sqr(u-N/2.)+sqr(v-M/2.));
	};

	//filters
	auto H0 = [](float u, float v) {
		return 1;
	};

	auto H1 = [&](float u, float v) {
		return (D(u, v) <= radius) ? 1 : 0;
	};

	auto inv_H1 = [&](float u, float v) {
		return 1-H1(u, v);
	};

	auto H2 = [&](float u, float v) {
		return exp(-sqr(D(u,v))/(2*sqr(radius)));
	};

	auto inv_H2 = [&](float u, float v) {
		return 1-H2(u, v);
	};

	output(complexImageDFT, H0,     ""); //without filter
	output(complexImageDFT, H1,     "perfect");
	output(complexImageDFT, inv_H1, "inverted_perfect");
	output(complexImageDFT, H2,     "gauss");
	output(complexImageDFT, inv_H2, "inverted_gauss");

	std::cout << "Done!\n";
}