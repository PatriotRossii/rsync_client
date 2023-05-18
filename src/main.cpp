#include <librsync.h>

#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <boost/program_options.hpp>

namespace po = boost::program_options;


FILE* open_file(const std::string& filename, const char* mode) {
	std::FILE* file = std::fopen(filename.c_str(), mode);
	if (!file) {
		throw filename;
	}
	return file;
}


int main(int argc, char* argv[]) {
	po::options_description desc{"Allowed options"};
	po::variables_map vm;
	po::positional_options_description pd;

	std::string source, second_source, output;

	pd.add("source", 1).add("output", 1).add("second_source", 1);
	desc.add_options()
		("help", "produce help message")
		("source", po::value<std::string>(&source))
		("second_source", po::value<std::string>(&second_source), "must be set if operation is signature or operation is patch")
		("output", po::value<std::string>(&output))
		("signature", "generate the signature of a source file and write it out to output")
		("delta", "generate a delta between a signature and a source file into a output file")
		("patch", "apply a patch, relative to a source file, into a output file")
	;

	po::store(po::command_line_parser(argc, argv).options(desc).positional(pd).run(), vm);
	po::notify(vm);

	if (vm.count("help") || !source.size() || !output.size()) {
		std::cout << "Usage: rsync_client [OPTIONS]... SOURCE OUTPUT [SECOND_SOURCE]" << '\n';
		std::cout << desc;
		return EXIT_SUCCESS;
	}

	if (!source.size() || !output.size()) {
		return EXIT_FAILURE;
	}

	if (vm.count("signature")) {
		FILE *source_file = open_file(source, "r"),
			 *destination_file = open_file(output, "w")
		;

		auto result = rs_sig_file(
			source_file, destination_file,
			0, 0, static_cast<rs_magic_number>(0), nullptr
		);
		std::cout << "signature: " << rs_strerror(result) << '\n';
	} else if (vm.count("delta")) {
		FILE *source_file = open_file(source, "r"),
			 *destination_file = open_file(output, "w"),
			 *signature_file = open_file(second_source, "r")
		;

		if (!signature_file) {
			std::cout << "Failed to open " << second_source << '\n';
			return EXIT_FAILURE;
		}

		rs_signature_t* sig;
		rs_loadsig_file(signature_file, &sig, nullptr);

	    rs_build_hash_table(sig);
		auto result = rs_delta_file(
			sig, source_file, destination_file, nullptr
		);

		std::cout << "delta: " << rs_strerror(result) << '\n';
	} else if (vm.count("patch")) {
		FILE *source_file = open_file(source, "r"),
			 *destination_file = open_file(output, "w"),
			 *delta_file = open_file(second_source, "r")
		;

		if (!delta_file) {
			std::cout << "Failed to open " << second_source << '\n';
			return EXIT_FAILURE;
		}

		auto result = rs_patch_file(
			source_file, delta_file, destination_file, nullptr
		);
		std::cout << "patch: " << rs_strerror(result) << '\n';
	} else {
		std::cout << "Please, specify operation" << '\n';
	}
}