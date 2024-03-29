#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include "Participantes.h"
#include "Uteis.h"
#include "Eventos.h"

// CONCLUIDO

/*
*       -----------------------
*           CONGRESSISTAS
*       -----------------------
*/

int quantidade_congressistas(){

    FILE *congress;
    int quantidade = 0;
    congress = fopen("Arquivos\\congressistas.txt", "rb");

    if (congress == NULL){
        fclose(congress);
        return -1;
    } else {
        CONGRESSISTA cong;
        while (fread(&cong, sizeof(CONGRESSISTA), 1, congress) == 1){
            quantidade++;
        }
        fclose(congress);
        return quantidade;
    }
}

int listar_congressistas(){

    FILE *congress;
    congress = fopen("Arquivos\\congressistas.txt", "rb");

    if (congress == NULL){
        fclose(congress);
        return -1;
    } else {
        if (quantidade_congressistas() == 0){
            fclose(congress);
            return -2;
        } else {
            CONGRESSISTA cong;
            printf("LISTANDO:\n\n");
            while(fread(&cong, sizeof(CONGRESSISTA), 1, congress) == 1){
                printf("\tID: %d\n", cong.ID);
                printf("\tNome: %s", cong.nome);
                printf("\tCurso: %s", cong.curso);
                putchar('\n');
            }

            fclose(congress);
            return 0;
        }
    }
    fclose(congress);
}

int cadastrar_congressista(){

    FILE *congress;
    congress = fopen("Arquivos\\congressistas.txt", "ab");

    if (congress == NULL){
        fclose(congress);
        return -1;
    } else {
        if (quantidade_congressistas() >= 300){
            fclose(congress);
            return -2;
        } else {

            CONGRESSISTA cong;
            cong.ID = gerar_id_valido(5);

            setbuf(stdin, NULL);
            putchar('\n');
            putchar('\n');

            printf("Nome: ");
            fgets(cong.nome, 30, stdin);
            strcpy(cong.nome, strupr(cong.nome));
            setbuf(stdin, NULL);

            printf("Curso: ");
            fgets(cong.curso, 30, stdin);
            strcpy(cong.curso, strupr(cong.curso));
            setbuf(stdin, NULL);

            fwrite(&cong, sizeof(CONGRESSISTA), 1, congress);
            fclose(congress);
        }
    }
    fclose(congress);
    return 0;
}

int remover_congressista(){

    FILE *congress;
    FILE *congress2;

    congress = fopen("Arquivos\\congressistas.txt", "rb");
    congress2 = fopen("Arquivos\\temp.txt", "ab");

    if (congress == NULL || congress2 == NULL){
        fclose(congress);
        fclose(congress2);
        return -1;
    } else {
        CONGRESSISTA cong;
        if (quantidade_congressistas() == 0){
                fclose(congress);
                fclose(congress2);
                return -2; // Congressistas n�o cadastrados
            } else {

                listar_congressistas();
                int ID;
                printf("Digite o ID que Deseja Remover: ");
                scanf("%d", &ID);

                while (fread(&cong, sizeof(CONGRESSISTA), 1, congress) == 1){
                    if (ID != cong.ID){
                        fwrite(&cong, sizeof(CONGRESSISTA), 1, congress2);
                    }
                }

                // O arquivo s� ser� aberto se n�o houve falha.
                // Logo s� ser� preciso fechar se ele foi aberto.
                fclose(congress2);
                fclose(congress);
                remove("Arquivos\\congressistas.txt");
                rename("Arquivos\\temp.txt", "Arquivos\\congressistas.txt");
                remover_de_todos_eventos(ID);
                return 0;
        }
    }
    fclose(congress2);
    fclose(congress);
}

int editar_congressista(){

    FILE *congress;
    FILE *congress2;

    congress = fopen("Arquivos\\congressistas.txt", "rb");
    congress2 = fopen("Arquivos\\temp.txt", "ab");

    if (congress == NULL || congress2 == NULL){
        fclose(congress);
        fclose(congress2);
        return -1;
    } else {
        if (quantidade_congressistas() == 0){
            fclose(congress);
            fclose(congress2);
            return -2;
        } else {

        CONGRESSISTA aux;
        int editar_escolha;
        int ID;

        listar_congressistas();
        printf("Digite o ID que Deseja Remover: ");
        scanf("%d", &ID);

        do {
            printf("\n|1| - Alterar Nome\n");
            printf("|2| - Alterar Curso\n");
            printf("|3| - Alterar Tudo\n");
            printf("|4| - Voltar\n");
            printf(">>> ");
            scanf("%d", &editar_escolha);
            setbuf(stdin, NULL);

            switch(editar_escolha){
            case 1:

                while(fread(&aux, sizeof(CONGRESSISTA), 1, congress)){
                    if (aux.ID != ID){
                        fwrite(&aux, sizeof(CONGRESSISTA), 1, congress2);
                    } else {
                        printf("\nNovo Nome: ");
                        fgets(aux.nome, 30, stdin);
                        strcpy(aux.nome, strupr(aux.nome));
                        fwrite(&aux, sizeof(CONGRESSISTA), 1, congress2);
                    }
                }
                break;
            case 2:
                while(fread(&aux, sizeof(CONGRESSISTA), 1, congress)){
                    if (aux.ID != ID){
                        fwrite(&aux, sizeof(CONGRESSISTA), 1, congress2);
                    } else {
                        printf("\nNovo Curso: ");
                        fgets(aux.curso, 30, stdin);
                        strcpy(aux.curso, strupr(aux.curso));
                        fwrite(&aux, sizeof(CONGRESSISTA), 1, congress2);
                    }
                }
            case 3:
                while(fread(&aux, sizeof(CONGRESSISTA), 1, congress)){
                    if (aux.ID != ID){
                        fwrite(&aux, sizeof(CONGRESSISTA), 1, congress2);
                    } else {
                        printf("\nNovo Nome: ");
                        fgets(aux.nome, 30, stdin);
                        strcpy(aux.nome, strupr(aux.nome));
                        setbuf(stdin, NULL);

                        printf("\nNovo Curso: ");
                        fgets(aux.curso, 30, stdin);
                        strcpy(aux.curso, strupr(aux.curso));
                        setbuf(stdin, NULL);

                        fwrite(&aux, sizeof(CONGRESSISTA), 1, congress2);
                    }
                }
            }
        } while(editar_escolha != 4);

        }
    }
    fclose(congress);
    fclose(congress2);
    remove("Arquivos\\congressistas.txt");
    rename("Arquivos\\temp.txt", "Arquivos\\congressistas.txt");
    return 0;
    printf("\n==> CONGRESSISTA ALTERADO COM SUCESSO\n");
}

int remover_de_todos_eventos(int ID){

    FILE *palestras;
    FILE *palestras_aux;

    FILE *gp_discussoes;
    FILE *gp_discussoes_aux;

    FILE *cursos;
    FILE *cursos_aux;

    FILE *oficinas;
    FILE *oficinas_aux;


    int i;

    PALESTRA pale;
    palestras = fopen("Arquivos\\palestras.txt", "rb");
    palestras_aux = fopen("Arquivos\\palestras_aux.txt", "ab");

    while(fread(&pale, sizeof(PALESTRA), 1, palestras) == 1){
        for(i=0; i < 150; i++){
            if(pale.matriculas[i] == ID){
                pale.matriculas[i] = 0;
            }
        }
        fwrite(&pale, sizeof(PALESTRA), 1 , palestras_aux);
    }

    fclose(palestras);
    fclose(palestras_aux);
    remove("Arquivos\\palestras.txt");
    rename("Arquivos\\palestras_aux.txt", "Arquivos\\palestras.txt");

    // ------------

    GP_DISCUSSOES gp;
    gp_discussoes = fopen("Arquivos\\gp_discussoes.txt", "rb");
    gp_discussoes_aux = fopen("Arquivos\\gp_discussoes_aux.txt", "ab");

    while(fread(&gp, sizeof(GP_DISCUSSOES), 1, gp_discussoes) == 1){
        for(i=0; i < 50; i++){
            if(gp.matriculas[i] == ID){
                gp.matriculas[i] = 0;
            }
        }
        fwrite(&gp, sizeof(GP_DISCUSSOES), 1 , gp_discussoes_aux);
    }

    fclose(gp_discussoes);
    fclose(gp_discussoes_aux);
    remove("Arquivos\\gp_discussoes.txt");
    rename("Arquivos\\gp_discussoes_aux.txt", "Arquivos\\gp_discussoes.txt");


    //-------------

    CURSO cur;
    cursos = fopen("Arquivos\\cursos.txt", "rb");
    cursos_aux = fopen("Arquivos\\cursos_aux.txt", "ab");

    while(fread(&cur, sizeof(CURSO), 1, cursos) == 1){
        for(i=0; i < 40; i++){
            if(cur.matriculas[i] == ID){
                cur.matriculas[i] = 0;
            }
        }
        fwrite(&cur, sizeof(CURSO), 1 , cursos_aux);
    }

    fclose(cursos);
    fclose(cursos_aux);
    remove("Arquivos\\cursos.txt");
    rename("Arquivos\\cursos_aux.txt", "Arquivos\\cursos.txt");

    //-------------


    OFICINA ofic;
    oficinas = fopen("Arquivos\\oficinas.txt", "rb");
    oficinas_aux = fopen("Arquivos\\oficinas_aux.txt", "ab");

    while(fread(&ofic, sizeof(OFICINA), 1, oficinas) == 1){
        for(i=0; i < 20; i++){
            if(ofic.matriculas[i] == ID){
                ofic.matriculas[i] = 0;
            }
        }
        fwrite(&ofic, sizeof(OFICINA), 1 , oficinas_aux);
    }

    fclose(oficinas);
    fclose(oficinas_aux);
    remove("Arquivos\\oficinas.txt");
    rename("Arquivos\\oficinas_aux.txt", "Arquivos\\oficinas.txt");
}

int *informacoes_cadastro_eventos(int ID){

    FILE *palestras;
    FILE *cursos;
    FILE *oficinas;
    FILE *gp_discussao;

    palestras = fopen("Arquivos\\palestras.txt", "rb");
    cursos = fopen("Arquivos\\cursos.txt", "rb");
    oficinas = fopen("Arquivos\\oficinas.txt", "rb");
    gp_discussao = fopen("Arquivos\\gp_discussoes.txt", "rb");

    if (palestras == NULL ||
        cursos == NULL ||
        oficinas == NULL ||
        gp_discussao == NULL){
            return -1;
    }
    // O N�MERO DE EVENTOS QUE PODEM SER CADASTRADOS == 32 + 4 PARA GUARDAR A QUANTIDADE
    // TODAS AS INFORMA��ES AP�S O INDICE 3 (0,1,2,3) SE TRATA DOS ID's EM QUE ELES EST�O CADASTRADOS
    int *array_informacao = (int *) calloc(36, sizeof(int));
    if (array_informacao == NULL){
        return -1;
    }

    // INDICE 0 DO ARRAY GUARDA A QUANTIDADE DE PALESTRAS EM QUE ELE EST�
    PALESTRA pale;
    int i;
    int numero_verificador_cadastro = 0; // Serve para testar se foi inscrito ou n�o na palestra;

    while (fread(&pale, sizeof(PALESTRA), 1, palestras) == 1){
        for (i=0; i < 150; i++){
            if (pale.matriculas[i] == ID){
                //printf("%d\n", pale.matriculas[0]);
                //printf("teste");
                //getch();
                numero_verificador_cadastro += 1;
                array_informacao[0] += 1;
                // SE O CONGRESSISTA ESTIVER EM ALGUMA PALESTRA ADCIONE +1
            }
        }

        // Se foi cadastrado na palestra ele estar� != 0 se n�o, adiciona +1;
        if (numero_verificador_cadastro != 0){
            for (i=4; i < 36; i++){
                if (array_informacao[i] == 0){
                    array_informacao[i] = pale.ID;
                    numero_verificador_cadastro = 0;
                    break;
                }
            }
        }
    }

    fclose(palestras);
    numero_verificador_cadastro = 0;

    // INDICE 1 GUARDA QNT CURSOS CURSOS
    CURSO cur;
    while (fread(&cur, sizeof(CURSO), 1, cursos) == 1){

        for (i=0; i < 40; i++){
            if (cur.matriculas[i] == ID){
                numero_verificador_cadastro += 1;
                array_informacao[1] += 1;
                // SE O CONGRESSISTA ESTIVER EM ALGUMA PALESTRA ADCIONE +1
            }
        }

        // Se foi cadastrado na palestra ele estar� != 0 se n�o, adiciona +1;
        if (numero_verificador_cadastro != 0){
            for (i=4; i < 36; i++){
                if (array_informacao[i] == 0){
                    array_informacao[i] = cur.ID;
                    numero_verificador_cadastro = 0;
                    break;
                }
            }
        }
    }

    fclose(cursos);
    numero_verificador_cadastro = 0;

    // INDICE 2 GUARDA QNT OFICINAS
    OFICINA ofic;
    while (fread(&ofic, sizeof(OFICINA), 1, oficinas) == 1){

        for (i=0; i < 20; i++){
            if (ofic.matriculas[i] == ID){
                numero_verificador_cadastro += 1;
                array_informacao[2] += 1;
                // SE O CONGRESSISTA ESTIVER EM ALGUMA PALESTRA ADCIONE +1
            }
        }

        // Se foi cadastrado na palestra ele estar� != 0 se n�o, adiciona +1;
        if (numero_verificador_cadastro != 0){
            for (i=4; i < 36; i++){
                if (array_informacao[i] == 0){
                    array_informacao[i] = ofic.ID;
                    numero_verificador_cadastro = 0;
                    break;
                }
            }
        }
    }

    fclose(oficinas);
    numero_verificador_cadastro = 0;

    // INDICE 3 GUARDA QNT GP DE DISCUSSAO
    GP_DISCUSSOES gp;
    while (fread(&gp, sizeof(GP_DISCUSSOES), 1, gp_discussao) == 1){

        for (i=0; i < 50; i++){
            if (gp.matriculas[i] == ID){
                numero_verificador_cadastro += 1;
                array_informacao[3] += 1;
                // SE O CONGRESSISTA ESTIVER EM ALGUMA PALESTRA ADCIONE +1
            }
        }

        // Se foi cadastrado na palestra ele estar� != 0 se n�o, adiciona +1;
        if (numero_verificador_cadastro != 0){
            for (i=4; i < 36; i++){
                if (array_informacao[i] == 0){
                    array_informacao[i] = gp.ID;
                    numero_verificador_cadastro = 0;
                    break;
                }
            }
        }
    }

    fclose(gp_discussao);
    return array_informacao;
}

int inserir_congressista_evento(){

    FILE *palestras;
    FILE *palestras_aux;

    FILE *gp_discussoes;
    FILE *gp_discussoes_aux;

    FILE *cursos;
    FILE *cursos_aux;

    FILE *oficinas;
    FILE *oficinas_aux;


    int i;

    if (quantidade_congressistas() == 0){
        return -2; // CONGRESSISTA NAO CADASTRADO
    }

    int id_congressista;
    listar_congressistas();
    printf("==> ID: ");

    do {
        scanf("%d", &id_congressista);
    } while (verificar_ID(5, id_congressista) != 0);

    int *informacoes;
    informacoes = informacoes_cadastro_eventos(id_congressista);

    // Aluno j� participa de Curso/Oficina
    if (informacoes[1] == 1 || informacoes[2] == 1){

        printf("\nSe Inscrever Em: \n");
        printf("|1| - Palestra\n");
        printf("|2| - Grupo de Discussao\n");
        printf("|3| - Voltar\n");
        printf("==> ");

        int opcao;

        do {
            scanf("%d", &opcao);
        } while(opcao < 1 || opcao > 3);

        switch (opcao){
        case 1:
            if (quantidade_palestras() == 0){
                return -3; // N�o existem paletras cadastradas
            } else {

                int id_palestra;

                listar_palestras();
                printf("==> ID: ");

                do {
                    scanf("%d", &id_palestra);
                } while (verificar_ID(1, id_palestra) != 0);

                palestras = fopen("Arquivos\\palestras.txt", "rb");
                palestras_aux = fopen("Arquivos\\palestras_aux.txt", "ab");

                if (palestras == NULL || palestras_aux == NULL){
                    fclose(palestras);
                    fclose(palestras_aux);
                    return -1; // FALHA AO ABRIR O ARQUIVO
                }

                PALESTRA pale;
                while(fread(&pale, sizeof(PALESTRA), 1, palestras)){
                    if (pale.ID == id_palestra){
                        for (i=0; i < 150; i++){
                            if (pale.matriculas[i] == 0){
                                pale.matriculas[i] = id_congressista;
                                break;
                            }
                        }
                    }
                    fwrite(&pale, sizeof(PALESTRA), 1, palestras_aux);
                }

                fclose(palestras);
                fclose(palestras_aux);
                remove("Arquivos\\palestras.txt");
                rename("Arquivos\\palestras_aux.txt", "Arquivos\\palestras.txt");
            }
            break;
        case 2:
            if (quantidade_gp_discussao() == 0){
                return -4; // N�o existem GP de Discussao cadastradas
            } else {

                int id_gp;

                listar_gp_discussao();
                printf("==> ID: ");

                do {
                    scanf("%d", &id_gp);
                // ESSE ID VEM DA FUN��O GERAR ID DA BIBLIOTECA UTEIS
                } while (verificar_ID(2, id_gp) != 0);

                gp_discussoes = fopen("Arquivos\\gp_discussoes.txt", "rb");
                gp_discussoes_aux = fopen("Arquivos\\gp_discussoes_aux.txt", "ab");

                if (gp_discussoes == NULL || gp_discussoes_aux == NULL){
                    fclose(gp_discussoes);
                    fclose(gp_discussoes_aux);
                    return -1; // FALHA AO ABRIR O ARQUIVO
                }

                // Aqui fa�o a varredura do array de matriculas do evento
                // Se tiver local disponivel ele ir� inserir
                GP_DISCUSSOES gp;
                while(fread(&gp, sizeof(GP_DISCUSSOES), 1, gp_discussoes)){
                    if (gp.ID == id_gp){
                        for (i=0; i < 50; i++){
                            if (gp.matriculas[i] == 0){
                                gp.matriculas[i] = id_congressista;
                                break;
                            }
                        }
                    }
                    fwrite(&gp, sizeof(GP_DISCUSSOES), 1, gp_discussoes_aux);
                }

                fclose(gp_discussoes);
                fclose(gp_discussoes_aux);
                remove("Arquivos\\gp_discussoes.txt");
                rename("Arquivos\\gp_discussoes_aux.txt", "Arquivos\\gp_discussoes.txt");
            }
            break;
        case 3:
            break;
        }
    } else {

        printf("\nSe Inscrever Em: \n");
        printf("|1| - Palestra\n");
        printf("|2| - Grupo de Discussao\n");
        printf("|3| - Curso\n");
        printf("|4| - Oficina\n");
        printf("|5| - Voltar\n");
        printf("==> ");

        int opcao;

        do {
            scanf("%d", &opcao);
        } while(opcao < 1 || opcao > 3);

        switch(opcao){
        case 1:
            if(quantidade_palestras() == 0){
                return -3; // Palestras n�o cadastradas
            } else {
                int id_palestra;

                listar_palestras();
                printf("==> ID: ");

                do {
                    scanf("%d", &id_palestra);
                } while (verificar_ID(1, id_palestra) != 0);

                palestras = fopen("Arquivos\\palestras.txt", "rb");
                palestras_aux = fopen("Arquivos\\palestras_aux.txt", "ab");

                if (palestras == NULL || palestras_aux == NULL){
                    fclose(palestras);
                    fclose(palestras_aux);
                    return -1; // FALHA AO ABRIR O ARQUIVO
                }

                PALESTRA pale;
                while(fread(&pale, sizeof(PALESTRA), 1, palestras)){
                    if (pale.ID == id_palestra){
                        for (i=0; i < 150; i++){
                            if (pale.matriculas[i] == 0){
                                pale.matriculas[i] = id_congressista;
                                break;
                            }
                        }
                    }
                    fwrite(&pale, sizeof(PALESTRA), 1, palestras_aux);
                }

                fclose(palestras);
                fclose(palestras_aux);
                remove("Arquivos\\palestras.txt");
                rename("Arquivos\\palestras_aux.txt", "Arquivos\\palestras.txt");
            }
            break;
        case 2:
            if (quantidade_gp_discussao() == 0){
                return -4; // N�o existem GP de Discussao cadastradas
            } else {

                int id_gp;

                listar_gp_discussao();
                printf("==> ID: ");

                do {
                    scanf("%d", &id_gp);
                // ESSE ID VEM DA FUN��O GERAR ID DA BIBLIOTECA UTEIS
                } while (verificar_ID(2, id_gp) != 0);

                gp_discussoes = fopen("Arquivos\\gp_discussoes.txt", "rb");
                gp_discussoes_aux = fopen("Arquivos\\gp_discussoes_aux.txt", "ab");

                if (gp_discussoes == NULL || gp_discussoes_aux == NULL){
                    return -1; // FALHA AO ABRIR O ARQUIVO
                }

                // Aqui fa�o a varredura do array de matriculas do evento
                // Se tiver local disponivel ele ir� inserir
                GP_DISCUSSOES gp;
                while(fread(&gp, sizeof(GP_DISCUSSOES), 1, gp_discussoes)){
                    if (gp.ID == id_gp){
                        for (i=0; i < 50; i++){
                            if (gp.matriculas[i] == 0){
                                gp.matriculas[i] = id_congressista;
                                break;
                            }
                        }
                    }
                    fwrite(&gp, sizeof(GP_DISCUSSOES), 1, gp_discussoes_aux);
                }

                fclose(gp_discussoes);
                fclose(gp_discussoes_aux);
                remove("Arquivos\\gp_discussoes.txt");
                rename("Arquivos\\gp_discussoes_aux.txt", "Arquivos\\gp_discussoes.txt");
            }
            break;
        case 3:
            if (quantidade_cursos() == 0){
                return -5; // N�o existem cursos cadastradas
            } else {

                int id_curso;

                listar_cursos();
                printf("==> ID: ");

                do {
                    scanf("%d", &id_curso);
                } while (verificar_ID(3, id_curso) != 0);

                cursos = fopen("Arquivos\\cursos.txt", "rb");
                cursos_aux = fopen("Arquivos\\cursos_aux.txt", "ab");

                if (cursos == NULL || cursos_aux == NULL){
                    fclose(cursos);
                    fclose(cursos_aux);
                    return -1; // FALHA AO ABRIR O ARQUIVO
                }

                CURSO cur;
                while(fread(&cur, sizeof(CURSO), 1, cursos)){
                    if (cur.ID == id_curso){
                        for (i=0; i < 40; i++){
                            if (cur.matriculas[i] == 0){
                                cur.matriculas[i] = id_congressista;
                                break;
                            }
                        }
                    fwrite(&cur, sizeof(CURSO), 1, cursos_aux);
                    } else {
                        fwrite(&cur, sizeof(CURSO), 1, cursos_aux);
                    }
                }

                fclose(cursos);
                fclose(cursos_aux);
                remove("Arquivos\\cursos.txt");
                rename("Arquivos\\cursos_aux.txt", "Arquivos\\cursos.txt");
            }
            break;

        case 4:
            if (quantidade_oficinas() == 0){
                return -6; // N�o existem oficinas cadastradas
            } else {

                int id_oficina;

                listar_oficinas();
                printf("==> ID: ");

                do {
                    scanf("%d", &id_oficina);
                } while (verificar_ID(4, id_oficina) != 0);

                oficinas = fopen("Arquivos\\oficinas.txt", "rb");
                oficinas_aux = fopen("Arquivos\\oficinas_aux.txt", "ab");

                if (oficinas == NULL || oficinas_aux == NULL){
                    return -1; // FALHA AO ABRIR O ARQUIVO
                }

                OFICINA ofic;
                while(fread(&ofic, sizeof(OFICINA), 1, oficinas)){
                    if (ofic.ID == id_oficina){
                        for (i=0; i < 20; i++){
                            if (ofic.matriculas[i] == 0){
                                ofic.matriculas[i] = id_congressista;
                                break;
                            }
                        }
                    fwrite(&ofic, sizeof(OFICINA), 1, oficinas_aux);
                    } else {
                        fwrite(&ofic, sizeof(OFICINA), 1, oficinas_aux);
                    }
                }

                fclose(oficinas);
                fclose(oficinas_aux);
                remove("Arquivos\\oficinas.txt");
                rename("Arquivos\\oficinas_aux.txt", "Arquivos\\oficinas.txt");
            }
            break;
        case 5:
            break;
        }
    }
    return 0;
}

int remover_congressista_evento(){

    int i;

    if (quantidade_congressistas() == 0){
        return -2; // Se n�o foi cadastrado congressista retorna -2
    }

    int id_congressista;
    listar_congressistas();
    printf("==> ID: ");

    do {
        scanf("%d", &id_congressista);
    } while (verificar_ID(5, id_congressista) != 0);

    int *informacoes;
    informacoes = informacoes_cadastro_eventos(id_congressista);

    printf("Parcipa dos Eventos a Baixo: \n");

    for (i=4; i < 36; i++){
        if (informacoes[i] > 0 && informacoes[i] <= 100){
            printf("Palestra: %d\n", informacoes[i]);
        } else {
            if (informacoes[i] > 100 && informacoes[i] <= 200){
                printf("Grupo de Discussao: %d\n", informacoes[i]);
            } else {
                if (informacoes[i] > 200 && informacoes[i] <= 300){
                    printf("Curso: %d\n", informacoes[i]);
                } else if (informacoes[i] > 300 && informacoes[i] <= 400){
                    printf("Oficina: %d\n", informacoes[i]);
                }
            }
        }
    }
    printf("CASO NAO HADA ID, DIGITE 0\n");
    int escolha_id;
    int tipo_evento;

    printf("\nID: ");
    scanf("%d", &escolha_id);

    if (escolha_id >= 0 && escolha_id <= 100){
        tipo_evento = 1;
    } else if(escolha_id > 100 && escolha_id <= 200){
        tipo_evento = 2;
    } else if (escolha_id > 200 && escolha_id <= 300){
        tipo_evento = 3;
    } else if (escolha_id > 300 && escolha_id <= 400){
        tipo_evento = 4;
    } else if(escolha_id == 0){
        tipo_evento = 0;
    }

    FILE *palestras;
    FILE *palestras_aux;
    FILE *gp_discussoes;
    FILE *gp_discussoes_aux;
    FILE *cursos;
    FILE *cursos_aux;
    FILE *oficinas;
    FILE *oficinas_aux;

    switch(tipo_evento){
    case 0:
        return -100; // S� pra voltar
    case 1:

        palestras = fopen("Arquivos\\palestras.txt", "rb");
        palestras_aux = fopen("Arquivos\\palestras_aux.txt", "ab");

        if (palestras == NULL || palestras_aux == NULL){
            return -1;
        }

        PALESTRA pale;
        while(fread(&pale, sizeof(PALESTRA), 1, palestras) == 1){
            if (pale.ID == escolha_id){
                for (i=0; i < 150; i++){
                    if (pale.matriculas[i] == id_congressista){
                        pale.matriculas[i] = 0;
                    }
                }
            }

            fwrite(&pale, sizeof(PALESTRA), 1, palestras_aux);
        }


        fclose(palestras);
        fclose(palestras_aux);
        remove("Arquivos\\palestras.txt");
        rename("Arquivos\\palestras_aux.txt", "Arquivos\\palestras.txt");
        break;
    case 2:

        gp_discussoes = fopen("Arquivos\\gp_discussoes.txt", "rb");
        gp_discussoes_aux = fopen("Arquivos\\gp_discussoes_aux.txt", "ab");

        if (gp_discussoes == NULL || gp_discussoes_aux == NULL){
            return -1;
        }

        GP_DISCUSSOES gp;
        while(fread(&gp, sizeof(GP_DISCUSSOES), 1, gp_discussoes) == 1){
            if (gp.ID == escolha_id){
                for (i=0; i < 50; i++){
                    if (gp.matriculas[i] == id_congressista){
                        gp.matriculas[i] = 0;
                    }
                }
            }

            fwrite(&gp, sizeof(GP_DISCUSSOES), 1, gp_discussoes_aux);
        }


        fclose(gp_discussoes);
        fclose(gp_discussoes_aux);
        remove("Arquivos\\gp_discussoes.txt");
        rename("Arquivos\\gp_discussoes_aux.txt", "Arquivos\\gp_discussoes.txt");
        break;
    case 3:

        cursos = fopen("Arquivos\\cursos.txt", "rb");
        cursos_aux = fopen("Arquivos\\cursos_aux.txt", "ab");

        if (cursos == NULL || cursos_aux == NULL){
            return -1;
        }

        CURSO cur;
        while(fread(&cur, sizeof(CURSO), 1, cursos) == 1){
            if (cur.ID == escolha_id){
                for (i=0; i < 40; i++){
                    if (cur.matriculas[i] == id_congressista){
                        cur.matriculas[i] = 0;
                    }
                }
            }

            fwrite(&cur, sizeof(CURSO), 1, cursos_aux);
        }


        fclose(cursos);
        fclose(cursos_aux);
        remove("Arquivos\\cursos.txt");
        rename("Arquivos\\cursos_aux.txt", "Arquivos\\cursos.txt");
        break;
    case 4:

        oficinas = fopen("Arquivos\\oficinas.txt", "rb");
        oficinas_aux = fopen("Arquivos\\oficinas_aux.txt", "ab");

        if (oficinas == NULL || oficinas_aux == NULL){
            return -1;
        }

        OFICINA ofic;
        while(fread(&ofic, sizeof(OFICINA), 1, oficinas) == 1){
            if (ofic.ID == escolha_id){
                for (i=0; i < 40; i++){
                    if (ofic.matriculas[i] == id_congressista){
                        ofic.matriculas[i] = 0;
                    }
                }
            }

            fwrite(&ofic, sizeof(OFICINA), 1, oficinas_aux);
        }

        fclose(oficinas);
        fclose(oficinas_aux);
        remove("Arquivos\\oficinas.txt");
        rename("Arquivos\\oficinas_aux.txt", "Arquivos\\oficinas.txt");
        break;
    }

    free(informacoes);
    return 0;
}

/*
*       -----------------------
*           ORGANIZADORES
*       -----------------------
*/

int quantidade_organizadores(){

    FILE *org;
    int quantidade=0;
    org = fopen("Arquivos\\organizadores.txt", "rb");

    if (org == NULL){
        fclose(org);
        return -1;
    } else {
        ORGANIZADOR orga;
        while (fread(&orga, sizeof(ORGANIZADOR), 1, org) == 1){
            quantidade++;
        }
    }
    fclose(org);
    return quantidade;
}

int listar_organizadores(){

    FILE *org;
    org = fopen("Arquivos\\organizadores.txt", "rb");

    if (org == NULL){
        fclose(org);
        return -1;
    } else {
        ORGANIZADOR orga;
        if (quantidade_organizadores() == 0){
            fclose(org);
            return -2;
        } else {
            printf("\n\nListando: \n");
            while(fread(&orga, sizeof(ORGANIZADOR),1, org) == 1){
                printf("\tID: %d\n", orga.ID);
                printf("\tLogin: %s", orga.login);
                printf("\tSenha: %s", orga.senha);
                putchar('\n');
            }
            fclose(org);
        }
    }
    fclose(org);
    return 0;
}

int cadastrar_organizador(){

    FILE *org;
    org = fopen("Arquivos\\organizadores.txt", "ab");

    if (org == NULL){
        fclose(org);
        return -1;
    } else {
        ORGANIZADOR orga;
        // NUMERO 6 RECEBE UM ID DE ORGANIZADOR
        orga.ID = gerar_id_valido(6);

        setbuf(stdin, NULL);
        printf("\n\n");

        printf("Login: ");
        fgets(orga.login, 30, stdin);
        setbuf(stdin, NULL);

        printf("Senha: ");
        fgets(orga.senha, 30, stdin);
        setbuf(stdin, NULL);

        fwrite(&orga, sizeof(ORGANIZADOR), 1, org);
        fclose(org);
    }
    fclose(org);
    return 0;
}

int remover_organizador(){

    FILE *org;
    FILE *org_aux;
    org = fopen("Arquivos\\organizadores.txt", "rb");
    org_aux = fopen("Arquivos\\temp.txt", "ab");

    if (org == NULL || org_aux == NULL){
        fclose(org);
        fclose(org_aux);
        return -1;
    } else {
            ORGANIZADOR pessoa;

            if (quantidade_organizadores() == 0){
                return -2;
            } else {
                listar_organizadores();
                int ID;
                printf("Digite o ID que Deseja Remover: ");
                scanf("%d", &ID);

                while (fread(&pessoa, sizeof(ORGANIZADOR), 1, org) == 1){
                    if (ID != pessoa.ID){
                        fwrite(&pessoa, sizeof(ORGANIZADOR), 1, org_aux);
                    }
                }

                fclose(org);
                fclose(org_aux);
                remove("Arquivos\\organizadores.txt");
                rename("Arquivos\\temp.txt", "Arquivos\\organizadores.txt");
        }
    }
    fclose(org);
    fclose(org_aux);
    return 0;
}

int editar_organizador(){

    FILE *org;
    FILE *org_aux;

    org = fopen("Arquivos\\organizadores.txt", "rb");
    org_aux = fopen("Arquivos\\temp.txt", "ab");

    if (org == NULL || org_aux == NULL){
        fclose(org);
        fclose(org_aux);
        return -1;
    } else {
        ORGANIZADOR aux;
        int editar_escolha;
        int ID;
        if (quantidade_organizadores() == 0){
            fclose(org);
            fclose(org_aux);
            return -2;
        } else {

        listar_organizadores();
        printf("Digite o ID que Deseja Alterar: ");
        scanf("%d", &ID);

        do {
            printf("\n|1| - Alterar Login\n");
            printf("|2| - Alterar Senha\n");
            printf("|3| - Alterar Tudo\n");
            printf("|4| - Voltar\n");
            printf(">>> ");
            scanf("%d", &editar_escolha);
            setbuf(stdin, NULL);

            switch(editar_escolha){
            case 1:

                while(fread(&aux, sizeof(ORGANIZADOR), 1, org)){
                    if (aux.ID != ID){
                        fwrite(&aux, sizeof(ORGANIZADOR), 1, org_aux);
                    } else {
                        printf("\nNovo Login: ");
                        fgets(aux.login, 30, stdin);
                        fwrite(&aux, sizeof(ORGANIZADOR), 1, org_aux);
                    }
                }
                break;
            case 2:
                while(fread(&aux, sizeof(ORGANIZADOR), 1, org)){
                    if (aux.ID != ID){
                        fwrite(&aux, sizeof(ORGANIZADOR), 1, org_aux);
                    } else {
                        printf("\nNova Senha: ");
                        fgets(aux.senha, 30, stdin);
                        fwrite(&aux, sizeof(ORGANIZADOR), 1, org_aux);
                    }
                }
            case 3:
                while(fread(&aux, sizeof(ORGANIZADOR), 1, org)){
                    if (aux.ID != ID){
                        fwrite(&aux, sizeof(ORGANIZADOR), 1, org_aux);
                    } else {
                        printf("\nNovo Login: ");
                        fgets(aux.login, 30, stdin);
                        setbuf(stdin, NULL);

                        printf("\nNova Senha: ");
                        fgets(aux.senha, 30, stdin);
                        setbuf(stdin, NULL);

                        fwrite(&aux, sizeof(ORGANIZADOR), 1, org_aux);
                    }
                }
            }
        } while(editar_escolha != 4);

        }
    }
    fclose(org);
    fclose(org_aux);
    remove("Arquivos\\organizadores.txt");
    rename("Arquivos\\temp.txt", "Arquivos\\organizadores.txt");
    return 0;
}

/*
*       -----------------------
*           PALESTRANTES
*       -----------------------
*/

int quantidade_palestrantes(){

    FILE *pales;
    int quantidade=0;
    pales = fopen("Arquivos\\palestrantes.txt", "rb");

    if (pales == NULL){
        fclose(pales);
        return -1;
    } else {
        PALESTRANTE pa;
        while (fread(&pa, sizeof(PALESTRANTE), 1, pales) == 1){
            quantidade++;
        }
    }
    fclose(pales);
    return quantidade;
}

int listar_palestrantes(){

    FILE *pale;
    pale = fopen("Arquivos\\palestrantes.txt", "rb");

    if (pale == NULL){
        fclose(pale);
        return -1;
    } else {
        PALESTRANTE palest;
        if (quantidade_palestrantes() == 0){
            fclose(pale);
            return -2;
        } else {
            printf("\nListando: \n\n");
            while(fread(&palest, sizeof(PALESTRANTE),1, pale) == 1){
                printf("\tID: %d\n", palest.ID);
                printf("\tNome: %s", palest.nome);
                printf("\tFormacao: %s", palest.formacao);
                putchar('\n');
            }
        }
        fclose(pale);
    }
    fclose(pale);
    return 0;
}

// O RETORNO DA FUN��O FOI ALTERADO DEVIDO A FUN��O DE GP DE DISCUSSAO
PALESTRANTE *cadastrar_palestrante(){

    FILE *pale;
    pale = fopen("Arquivos\\palestrantes.txt", "ab");

    PALESTRANTE *palest;
    palest = (PALESTRANTE *) malloc(sizeof(PALESTRANTE));

    if (pale == NULL){
        fclose(pale);
        free(palest);
        return -1;
    } else {

        // 7 GERA UM ID DO TIPO PALESTRANTE
        palest->ID = gerar_id_valido(7);

        setbuf(stdin, NULL);
        printf("\n\n");

        printf("Nome: ");
        fgets(palest->nome, 30, stdin);
        strcpy(palest->nome, strupr(palest->nome));
        setbuf(stdin, NULL);

        printf("Formacao: ");
        fgets(palest->formacao, 30, stdin);
        strcpy(palest->formacao, strupr(palest->formacao));
        setbuf(stdin, NULL);

        fwrite(palest, sizeof(PALESTRANTE), 1, pale);

    }
    fclose(pale);
    return palest;
}

int remover_palestrante(){

    FILE *pale;
    FILE *pale_aux;

    pale = fopen("Arquivos\\palestrantes.txt", "rb");
    pale_aux = fopen("Arquivos\\temp.txt", "ab");

    if (pale == NULL || pale_aux == NULL){
        fclose(pale);
        fclose(pale_aux);
        return -1;
    } else {
        PALESTRANTE palest;

        if (quantidade_palestrantes() == 0){
            fclose(pale);
            fclose(pale_aux);
            return -2;
        }

        listar_palestrantes();
        printf("Digite o ID que Deseja Remover: ");
        int ID;
        scanf("%d", &ID);

        while (fread(&palest, sizeof(PALESTRANTE), 1, pale) == 1){
            if (ID != palest.ID){
                fwrite(&palest, sizeof(PALESTRANTE), 1, pale_aux);
            }
        }
        fclose(pale);
        fclose(pale_aux);
        remove("Arquivos\\palestrantes.txt");
        rename("Arquivos\\temp.txt", "Arquivos\\palestrantes.txt");
        remover_eventos_de_um_palestrante(ID);
        }
    fclose(pale);
    fclose(pale_aux);
    return 0;
}

int editar_palestrante(){

    FILE *pale;
    FILE *pale_aux;

    pale = fopen("Arquivos\\palestrantes.txt", "rb");
    pale_aux = fopen("Arquivos\\temp.txt", "ab");

    if (pale == NULL || pale_aux == NULL){
        fclose(pale);
        fclose(pale_aux);
        return -1;
    } else {
        PALESTRANTE aux;
        int editar_escolha;
        int ID;

        if (quantidade_palestrantes() == 0){
            fclose(pale);
            fclose(pale_aux);
            return -2;
        } else {

        setbuf(stdin, NULL);

        listar_palestrantes();
        printf("Digite o ID que Deseja Alterar: ");
        scanf("%d", &ID);

        do {
            printf("\n|1| - Alterar Nome\n");
            printf("|2| - Alterar Formacao\n");
            printf("|3| - Alterar Tudo\n");
            printf("|4| - Voltar\n");
            printf(">>> ");
            scanf("%d", &editar_escolha);
            setbuf(stdin, NULL);

            switch(editar_escolha){
            case 1:

                while(fread(&aux, sizeof(PALESTRANTE), 1, pale)){
                    if (aux.ID != ID){
                        fwrite(&aux, sizeof(PALESTRANTE), 1, pale_aux);
                    } else {
                        printf("\nNovo Nome: ");
                        fgets(aux.nome, 30, stdin);
                        strcpy(aux.nome, strupr(aux.nome));
                        fwrite(&aux, sizeof(PALESTRANTE), 1, pale_aux);
                    }
                }
                break;
            case 2:
                while(fread(&aux, sizeof(PALESTRANTE), 1, pale)){
                    if (aux.ID != ID){
                        fwrite(&aux, sizeof(PALESTRANTE), 1, pale_aux);
                    } else {
                        printf("\nNova Formacao: ");
                        fgets(aux.formacao, 30, stdin);
                        strcpy(aux.formacao, strupr(aux.formacao));
                        fwrite(&aux, sizeof(PALESTRANTE), 1, pale_aux);
                    }
                }
                break;
            case 3:
                while(fread(&aux, sizeof(PALESTRANTE), 1, pale)){
                    if (aux.ID != ID){
                        fwrite(&aux, sizeof(PALESTRANTE), 1, pale_aux);
                    } else {
                        printf("\nNovo Nome: ");
                        fgets(aux.nome, 30, stdin);
                        strcpy(aux.nome, strupr(aux.nome));
                        setbuf(stdin, NULL);

                        printf("Nova Formacao: ");
                        fgets(aux.formacao, 30, stdin);
                        strcpy(aux.formacao, strupr(aux.formacao));
                        setbuf(stdin, NULL);

                        fwrite(&aux, sizeof(PALESTRANTE), 1, pale_aux);
                    }
                }
            }
            break;
            setbuf(stdin, NULL);
        } while(editar_escolha != 4);
            fclose(pale);
            fclose(pale_aux);
        }
        fclose(pale);
        fclose(pale_aux);
    }
    fclose(pale);
    fclose(pale_aux);
    remove("Arquivos\\palestrantes.txt");
    rename("Arquivos\\temp.txt", "Arquivos\\palestrantes.txt");
    return 0;
}

int remover_eventos_de_um_palestrante(int ID){

    FILE *palestras;
    FILE *palestras_aux;

    FILE *gp_discussoes;
    FILE *gp_discussoes_aux;

    FILE *cursos;
    FILE *cursos_aux;

    FILE *oficinas;
    FILE *oficinas_aux;

    // -----------------

    palestras = fopen("Arquivos\\palestras.txt", "rb");
    palestras_aux = fopen("Arquivos\\palestras_aux", "ab");

    PALESTRA pale;

    while(fread(&pale, sizeof(PALESTRA), 1, palestras)==1){
        if (pale.professor_palestrante != ID){
            fwrite(&pale, sizeof(PALESTRA), 1,palestras_aux);
        }
    }

    fclose(palestras);
    fclose(palestras_aux);
    remove("Arquivos\\palestras.txt");
    rename("Arquivos\\palestras_aux.txt", "Arquivos\\palestras.txt");

    // -----------------

    cursos = fopen("Arquivos\\cursos.txt", "rb");
    cursos_aux = fopen("Arquivos\\cursos_aux.txt", "ab");

    CURSO cur;
    while (fread(&cur, sizeof(CURSO),1, cursos)==1){
        if(cur.professor_palestrante != ID){
            fwrite(&cur, sizeof(CURSO), 1, cursos_aux);
        }
    }

    fclose(cursos);
    fclose(cursos_aux);
    remove("Arquivos\\cursos.txt");
    rename("Arquivos\\cursos_aux.txt", "Arquivos\\cursos.txt");

    // -----------------

    oficinas = fopen("Arquivos\\oficinas.txt", "rb");
    oficinas_aux = fopen("Arquivos\\oficinas_aux.txt", "ab");

    OFICINA ofic;
    while (fread(&ofic, sizeof(OFICINA),1, oficinas)==1){
        if(ofic.professor_palestrante != ID){
            fwrite(&ofic, sizeof(OFICINA), 1, oficinas_aux);
        }
    }

    fclose(oficinas);
    fclose(oficinas_aux);
    remove("Arquivos\\oficinas.txt");
    rename("Arquivos\\oficinas_aux.txt", "Arquivos\\oficinas.txt");

    // -----------------


    gp_discussoes = fopen("Arquivos\\gp_discussoes.txt", "rb");
    gp_discussoes_aux = fopen("Arquivos\\gp_discussoes_aux.txt", "ab");

    int i;

    GP_DISCUSSOES gp;
    while (fread(&gp, sizeof(GP_DISCUSSOES),1, gp_discussoes)==1){
        for (i=0; i < 5; i++){
            if (gp.mesa[i] == ID){
                gp.mesa[i] = 0;
            }
        }
    }

    fclose(cursos);
    fclose(cursos_aux);
    remove("Arquivos\\oficinas.txt");
    rename("Arquivos\\oficinas_aux.txt", "Arquivos\\oficinas.txt");
}
