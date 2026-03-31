#pragma once
#include "Mesh.h";
#include "Shader.h";
#include "Texture.h";

class Sprite : public Mesh {

public :
	void Draw(Shader& shader, Texture2D& texture) {

        shader.SetInteger("texture", 0);
		glBindTexture(GL_TEXTURE_2D, texture.ID);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0); 
	}
};
