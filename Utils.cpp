#include "Utils.h"

#include "stb_image.h"

XY normalizeVec(float x, float y){
	float d = sqrtf(x * x + y * y);
	XY p = XY(x / d, y / d);
	return p;

}

int randInt(int min, int max){
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_int_distribution<> dist(min, max);

	return dist(e2);
}

int random_in_range(unsigned int min, unsigned int max)
{
	int base_random = rand(); /* in [0, RAND_MAX] */
	if (RAND_MAX == base_random) return random_in_range(min, max);
	/* now guaranteed to be in [0, RAND_MAX) */
	int range = max - min,
		remainder = RAND_MAX % range,
		bucket = RAND_MAX / range;
	/* There are range buckets, plus one smaller interval
	within remainder of RAND_MAX */
	if (base_random < RAND_MAX - remainder) {
		return min + base_random / bucket;
	}
	else {
		return random_in_range(min, max);
	}
}

float clamp(float value, float min, float max){
	if (max > min)	{
		if (value < min) 
			return min;
		else if (value > max) 
			return max;
		else 
			return value;
	} else {
		// Min/max swapped
		if (value < max) 
			return max;
		else if (value > min) 
			return min;
		else 
			return value;
	}
}

long currentTimeMillis()
{
#ifdef WIN32    
	return timeGetTime();
#else
	timeval t;
	gettimeofday(&t, NULL);

	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
#endif    
}

void loadTexture(char *filename, int tipo, GLuint textureArray[], int index){
	if (!filename)
		return;

	int w, h, n;
	unsigned char* imagem = stbi_load(filename, &w, &h, &n, 0);

	if (imagem == NULL){
		std::cout << "Error loading texture '" << filename << std::endl;
		return;
	}

	glGenTextures(1, &textureArray[index]);
	glBindTexture(GL_TEXTURE_2D, textureArray[index]);
	if (tipo == 1){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, imagem);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else{
		GLint ani;
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ani);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, ani);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, imagem);
	}

	stbi_image_free(imagem);
}

bool ReadFile(const char* pFileName, std::string& outFile)
{
	std::ifstream f(pFileName);

	bool ret = false;

	if (f.is_open()) {
		std::string line;
		while (std::getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		ret = true;
	}
	else {
		std::cout << "Erro ao abrir o ficheiro: " << pFileName << std::endl;
	}

	return ret;
}

void DrawCircle(float cx, float cy, float h, float r, int num_segments, RGBf rgb)
{
	float theta = 2 * 3.1415926 / float(num_segments);
	float tangetial_factor = tanf(theta);//calculate the tangential factor 

	float radial_factor = cosf(theta);//calculate the radial factor 

	float x = r;//we start at angle = 0 

	float y = 0;
	glColor3f(rgb.r, rgb.g, rgb.b);
	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments; ii++)
	{
		glVertex3f(x + cx, h, y + cy);//output vertex 

		float tx = -y;
		float ty = x;

		//add the tangential vector 

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		//correct using the radial factor 

		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
}