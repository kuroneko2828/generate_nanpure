let question;
let answer;

get_mondai();
function init(){
    const table = document.getElementById("nanpure");
    //table.innerHTML = "<tr><td>1</td><td>2</td></tr><tr><td>3</td><td>4</td></tr>";
    for(let i = 0; i < 9; i ++){
        let tr = document.createElement("tr");
        for(let j = 0; j < 9; j ++){
            let td = document.createElement("td");
            if (question[i][j] != 0){
                td.textContent = question[i][j];
            }else{
                td.textContent = null;
                td.classList.add("click_ok")
                td.onclick = select_masu;
            }
            tr.appendChild(td);
        }
        table.appendChild(tr);
    }
    const kouho = document.getElementById("kouho");
    let tr = document.createElement("tr");
    let td = document.createElement("td");
    td.onclick = select_kouho;
    tr.appendChild(td)
    td.onclick = select_kouho;
    for (let i = 0; i < 9; i  ++){
        let td = document.createElement("td");
        td.textContent = i+1;
        td.onclick = select_kouho;
        tr.appendChild(td)
    }
    kouho.appendChild(tr)
}

function get_mondai(){
    $.getJSON("json/nanpure_easy.json")
    .done(function (json) {
        var l = json.length;
        var a = Math.floor(Math.random()*l);
        question = json[a].mondai;
        answer = json[a].kaitou;
    }).fail(function () {
        // 失敗
        question = [
            [3, 7, 0, 1, 8, 5, 4, 2, 0],
            [1, 0, 4, 0, 9, 2, 6, 5, 7],
            [0, 0, 0, 0, 4, 0, 0, 3, 8],
            [0, 4, 0, 0, 0, 9, 3, 0, 0],
            [0, 3, 0, 0, 7, 0, 0, 9, 0],
            [0, 0, 5, 4, 0, 0, 0, 7, 0],
            [4, 1, 0, 0, 2, 0, 0, 0, 0],
            [5, 2, 7, 8, 6, 0, 9, 0, 3],
            [0, 6, 3, 9, 1, 7, 0, 4, 5],
        ];
    }).always(function () {
        // 常に最後に処理
        init();
    });
}

let masu = null;

function select_masu(e){
    if (masu != e.target){
        if (masu != null){
            masu.classList.remove("clicked");
        }
        masu = e.target;
        masu.classList.add("clicked");
    }else{
        masu.classList.remove("clicked");
        masu = null;
    }
}

function select_kouho(e){
    if (masu != null){
        masu.textContent =  e.target.textContent;
    }
}