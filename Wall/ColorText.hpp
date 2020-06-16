//
//  ColorText.hpp
//  Wall
//
//  Created by Nicolas Fricker on 6/16/20.
//  Copyright © 2020 nadp. All rights reserved.
//

#pragma once
#ifndef ColorText_h
#define ColorText_h

#include <iostream>
#include <string>

namespace cT {
	namespace {
		const char* init = "\x1b[";
		const char* term = "m";
	};
	namespace fG {
		const char* black = "30";
		const char* red = "31";
		const char* green = "32";
		const char* yellow = "33";
		const char* blue = "34";
		const char* magenta = "35";
		const char* cyan = "36";
		const char* white = "37";
		const char* defaultc = "39";
	};
	
	namespace bG {
		const char* black = "40";
		const char* red = "41";
		const char* green = "42";
		const char* yellow = "43";
		const char* blue = "44";
		const char* magenta = "45";
		const char* cyan = "46";
		const char* white = "47";
	};
	
	namespace sT {
		const char* normal = "0";
		const char* bold = "1";
		const char* dim = "2";
	};
	
	std::string getColor(const char* forground = fG::defaultc, const char* background = "") {
		std::string str;
		str += init;
		str += forground;
		if (strcmp(background, "")) {
			str += ";";
			str += background;
		}
		str += term;
		return str;
	}
	
	std::string getStyle(const char* style = sT::normal) {
		std::string str;
		str += init;
		str += style;
		str += term;
		if (strcmp(style, sT::normal)) {
			str += init;
			str += sT::normal;
			str += term;
		}
		return str;
	}
	
	std::string print(const char* text, const char* forground = fG::defaultc, const char* style = sT::normal, const char* background = "") {
		std::string str;
		str += getColor(forground, background);
		str += text;
		str += getStyle(style);
		return str;
	}
};

#endif /* ColorText_h */
