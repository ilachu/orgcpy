if [[ -z $1  ]]; then
	dir=$(pwd);
else
	dir=$1
fi

alpha "$(ls $dir)";
